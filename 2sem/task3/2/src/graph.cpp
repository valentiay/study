//
// Created by valentiay on 4/21/17.
//

#include "../include/graph.h"

Graph::Graph(unsigned int size) :
        size_(size),
        matrix_(size_, vector<double>(size_, 0.0)) {}



void Graph::addEdge(unsigned from, unsigned to, double weight)
{
    matrix_[from][to] = weight;
    matrix_[to][from] = weight;
}



unsigned Graph::getSize() const
{
    return size_;
}



void Graph::getRelated(unsigned vertex, vector<pair_ud> &vertices) const
{
    vertices.clear();
    for(unsigned i = 0; i < size_; i++){
        if(matrix_[vertex][i] != 0)
            vertices.push_back(make_pair(i, matrix_[vertex][i]));
    }
}



double Graph::getWeight(unsigned from, unsigned to) const
{
    return matrix_[from][to];
}



void Graph::print()
{
    for(int i = 0; i < size_; i++){
        for(int j = 0; j < size_; j++){
            std::cout << std::setw(7) << matrix_[i][j] << ' ';
        }
        std::cout << '\n';
    }
}