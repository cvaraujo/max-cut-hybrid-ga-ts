cmake_minimum_required(VERSION 3.5)
project(GA_TS)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(GA_TS
        include/Algorithm.h
        include/Graph.h
        include/Includes.h
        include/Individual.h
        include/Population.h
        main.cpp
        src/Algorithm.cpp
        src/Graph.cpp
        src/Individual.cpp
        src/Population.cpp)
