//
// Created by valentiay on 4/22/17.
//

#ifndef INC_2_TSP_MST_H
#define INC_2_TSP_MST_H

#include "graph.h"
#include "MST.h"

// Function object for solution using MST
class MinTourLenMST{
public:
    // Returns estimated length of traveling salesman tour
    double operator()(const Graph & graph);

private:
    // Function for finding Euler path in graph
    void DFS(const Graph & mst, unsigned dot);

    // Stores tour found in DFS
    vector<unsigned> tour_;
    // Stores used in DFS
    vector<bool> used_;
    // Number of vertices
    unsigned n_;
};

#endif //INC_2_TSP_MST_H
