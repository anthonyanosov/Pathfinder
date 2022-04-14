/*
    pathfinder.cpp
        
    Method implementations for the pathfinder class.
    
    assignment: CSCI 262 Project - Pathfinder        

    author: Anthony Anosov
*/

#include <iostream>
#include <fstream>
#include "pathfinder.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// constructor - optional, in case you need to initialize anything
pathfinder::pathfinder() { ; }

// public run() method - invokes private methods to do everything;
// returns the total elevation change cost of the best optimal path.
int pathfinder::run(string data_file_name, string save_file_name, bool use_recursion) {
    // suggested implementation - modify as desired!
    _use_recursion = use_recursion;
    if (!_read_data(data_file_name)) {
        cout << "Error reading map data from \"" << data_file_name << "\"!" << endl;
        return -1;
    }

    _draw_map();

    int best_cost = _draw_paths();

    _save_image(save_file_name);

    return best_cost;
}

/*******************/
/* PRIVATE METHODS */
/*******************/

// _read_data - read and store width, height, and elevation data from the
// provided data file; return false if something goes wrong
bool pathfinder::_read_data(string data_file_name) {
    string ncols;
    string nrows;
    ifstream fin;
    fin.open(data_file_name);
    fin >> ncols;
    fin >> _width;
    fin >> nrows;
    fin >> _height;
    _elevations = vector<vector<int>>(_height, vector<int>(_width));
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            fin >> _elevations[i][j];
        }
    }
    fin.close();
    return true;
}

// _draw_map - draw the elevation data as grayscale values on our Picture
// object.
void pathfinder::_draw_map() {
    int maximumElevation = 0;
    int minimumElevation = INT_MAX;
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            if (_elevations[i][j] > maximumElevation) {
                maximumElevation = _elevations[i][j];
            }
            if (_elevations[i][j] < minimumElevation) {
                minimumElevation = _elevations[i][j];
            }
        }
    }
    _image.set(_height - 1, _width - 1, 0, 0, 0); // Helping image library pre-allocate memory it needs
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            int gray = ((_elevations[i][j] - minimumElevation) * 255) / (maximumElevation - minimumElevation);
            _image.set(i, j, gray, gray, gray);
        }
    }
}

// _draw_paths() - find and draw optimal paths from west to east using either recursion or dynamic programming
// return the best optimal path cost
int pathfinder::_draw_paths() {
    int green;
    int red;
    int blue;
    int opt_cost = _use_recursion ? calculate_recursively() : calculate_dynamically();
    for (int r = 0; r < _height; r++) {
        red = 0;
        green = 0;
        blue = 255;
        int row = r;
        for (int c = 0; c < _width; c++) {
            _image.set(row, c, red, green, blue);
            switch (_directions[row][c]) {
                case 'n':
                    row -= 1;
                    break;
                case 's':
                    row += 1;
                    break;
                default:
                    row = row;
            }
        }
    }
    for (int r = 0; r < _height; r++) {
        red = 255;
        green = 255;
        blue = 0;
        int row = r;
        if (_costs[r][0] == opt_cost) {
            for (int c = 0; c < _width; c++) {
                _image.set(row, c, red, green, blue);
                switch (_directions[row][c]) {
                    case 'n':
                        row -= 1;
                        break;
                    case 's':
                        row += 1;
                        break;
                    default:
                        row = row;
                }
            }
        }
    }
    return opt_cost;
}

int pathfinder::calculate_recursively() {
    vector<int> optimalCosts;
    for (int i = 0; i < _height; i++) {
        optimalCosts.push_back(_optCostToEast(i, 0));
    }
    int minimumCost = *min_element(optimalCosts.begin(), optimalCosts.end());
    return minimumCost;
}

// Recursive Function

int pathfinder::_optCostToEast(int y, int x) {
    int bestCost = INT_MAX;
    int cost = 0;

    if (isMoveEastPossible(x)) {
        if (isNorthEastMovePossible(y)) {
            cost = calculateMoveCostRecursively(y, x, y - 1, x + 1);
            bestCost = findBetterCost(bestCost, cost);
        }
        cost = calculateMoveCostRecursively(y, x, y, x + 1);
        bestCost = findBetterCost(bestCost, cost);
        if (isSouthEastMovePossible(y)) {
            cost = calculateMoveCostRecursively(y, x, y + 1, x + 1);
            bestCost = findBetterCost(bestCost, cost);
        }
    } else {
        bestCost = 0;
    }
    return bestCost;
}

// My Own Methods

int pathfinder::calculateMoveCostRecursively(int y, int x, int i, int j) {
    return abs(_elevations[y][x] - _elevations[i][j]) + _optCostToEast(i, j);
}

int pathfinder::findBetterCost(int cost1, int cost2) {
    return cost1 > cost2 ? cost2 : cost1;
}

int pathfinder::calculate_dynamically() {
    _directions = vector<vector<char>>(_height, vector<char>(_width - 1));
    _costs = vector<vector<int>>(_height, vector<int>(_width, INT_MAX));
    int cost;

    for (int c = _width - 1; c >= 0; c--) {
        for (int r = 0; r < _height; r++) {
            if (isMoveEastPossible(c)) {
                if (isNorthEastMovePossible(r)) {
                    cost = calculateMoveCostDynamically(r, c, r - 1, c + 1);
                    if (cost < _costs[r][c]) {
                        _costs[r][c] = cost;
                        _directions[r][c] = 'n';
                    }
                }
                if (isSouthEastMovePossible(r)) {
                    cost = calculateMoveCostDynamically(r, c, r + 1, c + 1);
                    if (cost < _costs[r][c]) {
                        _costs[r][c] = cost;
                        _directions[r][c] = 's';
                    }
                }
                cost = calculateMoveCostDynamically(r, c, r, c + 1);
                if (cost < _costs[r][c]) {
                    _costs[r][c] = cost;
                    _directions[r][c] = 'e';
                }
            } else {
                _costs[r][c] = 0;
            }
        }
    }
    vector<int> optimalCosts;
    for (int i = 0; i < _height; i++) {
        optimalCosts.push_back(_costs[i][0]);
    }
    int minimumCost = *min_element(optimalCosts.begin(), optimalCosts.end());
    return minimumCost;
}

int pathfinder::calculateMoveCostDynamically(int y, int x, int i, int j) {
    return abs(_elevations[y][x] - _elevations[i][j]) + _costs[i][j];
}

bool pathfinder::isMoveEastPossible(int x) const {
    return x < _width - 1;
}

bool pathfinder::isSouthEastMovePossible(int y) const {
    return y < _height - 1;
}


bool pathfinder::isNorthEastMovePossible(int y) const {
    return y > 0;
}

// _save_image - we'll do this one for you :)
void pathfinder::_save_image(string save_file_name) {
    _image.save(save_file_name);
}

