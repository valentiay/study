//
// Created by valentiay on 16.02.17.
//

#include "../include/CSetGraph.h"

CSetGraph::CSetGraph(unsigned int verticesNumber):
    verticesNumber_(verticesNumber),
    sets_(verticesNumber_)
{}



CSetGraph::CSetGraph(const IGraph * graph):
    verticesNumber_(graph->VerticesCount()),
    sets_(verticesNumber_)
{
    for(int i = 0; i < verticesNumber_; i++){
        vector<int> vertices;
        graph->GetNextVertices(i, vertices);
        for(int j : vertices)
            AddEdge(i, j);
    }
}



void CSetGraph::AddEdge(int from, int to){
    sets_[from].insert(to);
}



int CSetGraph::VerticesCount() const{
    return verticesNumber_;
}



void CSetGraph::GetNextVertices(int vertex, vector<int> & vertices) const{
    for(int i : sets_[vertex])
        vertices.push_back(i);
}



void CSetGraph::GetPrevVertices(int vertex, vector<int> & vertices) const{
    for(int i = 0; i < verticesNumber_; i++)
        if(sets_[i].find(vertex) != sets_[i].end())
            vertices.push_back(i);
}