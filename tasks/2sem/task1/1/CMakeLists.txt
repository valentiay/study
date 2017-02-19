cmake_minimum_required(VERSION 3.6)
project(task1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp include/CListGraph.h include/IGraph.h include/CMatrixGraph.h src/CListgraph.cpp src/CMatrixGraph.cpp include/CSetGraph.h src/CSetGraph.cpp include/CArcGraph.h src/CArcGraph.cpp)
add_executable(task1 ${SOURCE_FILES} include/CListGraph.h src/CListgraph.cpp)