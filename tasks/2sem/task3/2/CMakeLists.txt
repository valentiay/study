cmake_minimum_required(VERSION 3.7)
project(2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/graph.cpp include/graph.h src/MST.cpp include/MST.h include/dots.h src/dots.cpp include/TSP_MST.h include/TSP_BF.h src/TSP_BF.cpp include/checker.h src/TSP_MST.cpp src/checker.cpp)
add_executable(2 ${SOURCE_FILES})