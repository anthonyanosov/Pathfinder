/*
 * main.cpp
 *
 * Implements a basic user interface for pathfinder.
 *
 * assignment: CSCI 262 Project - Pathfinder
 *
 * author: C. Painter-Wakefield
 *
 * last modified: 1/18/2020
 *
 * You should not need to modify anything in this file.
 */

#include <iostream>
#include <string>
#include <fstream>

#include "pathfinder.h"

using namespace std;

// utility method to obtain a filename and test that it is valid
// should really use C++17's filesystem library!
string get_file_name(string msg, bool input) {
    fstream file;
    ios_base::openmode mode;

    if (input) mode = ios::in;
    else mode = ios::out;

    string file_name;
    cout << msg;
    getline(cin, file_name);

    // try to open the file
    file.open(file_name, mode);
    while (!file) {
        cerr << "Error opening file \"" << file_name << "\"" << '\n';
        cerr << "Did you remember to move the data/ directory to cmake-build-debug?" << endl;

        file.clear();
        cout << endl;
        cout << msg;
        getline(cin, file_name);
        file.open(file_name, mode);
    }

    file.close();

    return file_name;
}

// basic user interface for pathfinder project
int main(int argc, char *argv[]) {
    cout << "Welcome to Pathfinder!" << endl;
    cout << "======================" << endl << endl;

    // get the filename to process first
    string data_file_name;
    data_file_name = get_file_name("Please enter the map data filename, e.g., \"data/colorado.dat\": ", true);

    cout << endl;

    // Get whether or not to use recursion
    string solution_choice;
    cout << "Use recursion (r) or dynamic programming (d): ";
    getline(cin, solution_choice);

    while (solution_choice != "r" && solution_choice != "d") {
        cerr << solution_choice << " is not an option, please respond with (r) or (d): ";
        getline(cin, solution_choice);
    }


    // get the save file name
    string save_file_name;
    save_file_name = get_file_name("Please enter the output filename, e.g., \"solution.png\": ", false);

    // create a pathfinder object 
    pathfinder p;

    // draw map and find paths, returning best optimal path cost
    int optimal = p.run(data_file_name, save_file_name, solution_choice == "r");

    if (optimal > 0) {
        cout << "Best optimal path total: " << optimal << endl;
    }

    return 0;
}
