//
// Created by valentiay on 16.02.17.
//

#include "../include/CListGraph.h"

CListGraph::CListGraph(unsigned int verticesNumber):
    verticesNumber_(verticesNumber),
    in_(verticesNumber_),
    out_(verticesNumber_)
{}



CListGraph::CListGraph(const IGraph * graph):
    verticesNumber_(graph->VerticesCount()),
    in_(verticesNumber_),
    out_(verticesNumber_)
{
for(int i = 0; i < verticesNumber_; i++){
    vector<int> vertices;
    graph->GetNextVertices(i, vertices);
    for(int j : vertices)
        AddEdge(i, j);
    }
}



void CListGraph::AddEdge(int from, int to){
    // Checking if edge already exists
    if(in_.size() < out_.size()){
        for(int i : in_[to])
            if(i == from)
                return;
    }
    else{
        for(int i : out_[from])
            if(i == to)
                return;
    }

    in_[to].push_back(from);
    out_[from].push_back(to);
}



int CListGraph::VerticesCount() const{
    return verticesNumber_;
}



void CListGraph::GetNextVertices(int vertex, vector<int> & vertices) const{
    for(int i : out_[vertex])
        vertices.push_back(i);
}



void CListGraph::GetPrevVertices(int vertex, vector<int> & vertices) const{
    for(int i : in_[vertex])
        vertices.push_back(i);
}