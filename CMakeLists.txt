cmake_minimum_required(VERSION 3.17)
project(project_1_starter_code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cmake-build-debug/data)

add_executable(project_1_starter_code
        cmake-build-debug/data/colorado.dat
        cmake-build-debug/data/example.dat
        cmake-build-debug/data/example2.dat
        cmake-build-debug/data/grand_canyon.dat
        cmake-build-debug/data/grand_canyon_hires.dat
        cmake-build-debug/data/kilimanjaro.dat
        cmake-build-debug/data/kilimanjaro_hires.dat
        lodepng.cpp
        lodepng.h
        main.cpp
        pathfinder.cpp
        pathfinder.h
        picture.cpp
        picture.h)
