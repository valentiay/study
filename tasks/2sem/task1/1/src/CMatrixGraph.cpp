//
// Created by valentiay on 16.02.17.
//

#include <iostream>
#include "../include/CMatrixGraph.h"

CMatrixGraph::CMatrixGraph(unsigned int verticesNumber):
    verticesNumber_(verticesNumber),
    edges_(verticesNumber_)
{
    for(vector<bool> & i : edges_)
        i.assign(verticesNumber_, false);
}



CMatrixGraph::CMatrixGraph(const IGraph * graph):
    verticesNumber_(graph->VerticesCount()),
    edges_(verticesNumber_)
{
    for(vector<bool> & i : edges_)
        i.assign(verticesNumber_, false);

    for(int i = 0; i < verticesNumber_; i++){
        vector<int> vertices;
        graph->GetNextVertices(i, vertices);
        for(int j : vertices)
            AddEdge(i, j);
    }
}



void CMatrixGraph::AddEdge(int from, int to){
    edges_[from][to] = true;
}



int CMatrixGraph::VerticesCount() const{
    return verticesNumber_;
}



void CMatrixGraph::GetNextVertices(int vertex, vector<int> & vertices) const{
    vertices.clear();
    for(int i = 0; i < verticesNumber_; i++)
        if(edges_[vertex][i])
            vertices.push_back(i);
}



void CMatrixGraph::GetPrevVertices(int vertex, vector<int> & vertices) const{
    vertices.clear();
    for(int i = 0; i < verticesNumber_; i++)
        if(edges_[i][vertex])
            vertices.push_back(i);
}