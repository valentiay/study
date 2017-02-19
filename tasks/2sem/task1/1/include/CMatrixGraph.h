//
// Created by valentiay on 16.02.17.
//

#ifndef TASK1_CMATRIXGRAPH_H
#define TASK1_CMATRIXGRAPH_H

#include "IGraph.h"

class CMatrixGraph : public IGraph{
public:

                    CMatrixGraph(unsigned int verticesNumber);

                    CMatrixGraph(const IGraph * graph);


    virtual void    AddEdge(int from, int to) override;


    virtual int     VerticesCount() const override;


    virtual void    GetNextVertices(int vertex,
                                    vector<int> & vertices) const override;

    virtual void    GetPrevVertices(int vertex,
                                    vector<int> & vertices) const override;

private:

    unsigned int            verticesNumber_;

    vector<vector<bool>>    edges_;

};

#endif //TASK1_CMATRIXGRAPH_H
