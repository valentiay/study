//
// Created by valentiay on 16.02.17.
//

#include "../include/CArcGraph.h"

CArcGraph::CArcGraph(unsigned int verticesNumber):
    verticesNumber_(verticesNumber)
{}



CArcGraph::CArcGraph(const IGraph * graph):
    verticesNumber_(graph->VerticesCount())
{
    for(int i = 0; i < verticesNumber_; i++){
        vector<int> vertices;
        graph->GetNextVertices(i, vertices);
        for(int j : vertices)
            AddEdge(i, j);
    }
}



void CArcGraph::AddEdge(int from, int to){
    // Checking if edge already exists
    for(const Edge & i : edges_)
        if(i.from == from && i.to == to)
            return;

    Edge edge(from, to);
    edges_.push_back(edge);
}



int CArcGraph::VerticesCount() const{
    return verticesNumber_;
}



void CArcGraph::GetNextVertices(int vertex, vector<int> & vertices) const{
    for(const Edge & i : edges_)
        if(i.from == vertex)
            vertices.push_back(i.to);
}



void CArcGraph::GetPrevVertices(int vertex, vector<int> & vertices) const{
    for(const Edge & i : edges_)
        if(i.to == vertex)
            vertices.push_back(i.from);
}



CArcGraph::Edge::Edge(int from, int to):
    from(from),
    to(to)
{}