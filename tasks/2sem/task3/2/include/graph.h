//
// Created by valentiay on 4/21/17.
//

#ifndef INC_2_GRAPH_H
#define INC_2_GRAPH_H

#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>

using std::vector;
using std::pair;
using std::make_pair;
using std::min;

typedef pair<unsigned, double> pair_ud;
typedef pair<double, unsigned> pair_du;

class Graph {
public:
    Graph(unsigned verticesNumber);

    void addEdge(unsigned from, unsigned to, double weight);

    // Vertices - vector of pairs <vertex, weight>
    void getRelated(unsigned vertex, vector<pair_ud> &vertices) const;

    unsigned getSize() const;

    double getWeight(unsigned from, unsigned to) const;

    void print();

private:
    unsigned size_;
    vector<vector<double>> matrix_;
};


#endif //INC_2_GRAPH_H
