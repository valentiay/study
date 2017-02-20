//
// Created by valentiay on 16.02.17.
//

#ifndef TASK1_CSETGRAPH_H
#define TASK1_CSETGRAPH_H

#include <unordered_set>
#include "IGraph.h"

using std::unordered_set;

class CSetGraph : public IGraph{
public:

                    CSetGraph(unsigned int verticesNumber);

                    CSetGraph(const IGraph * graph);


    virtual void    AddEdge(int from, int to) override;


    virtual int     VerticesCount() const override;


    virtual void    GetNextVertices(int vertex,
                                    vector<int> & vertices) const override;

    virtual void    GetPrevVertices(int vertex,
                                    vector<int> & vertices) const override;

private:

    unsigned int                  verticesNumber_;

    vector<unordered_set<int>>    out_;

    vector<unordered_set<int>>    in_;

};

#endif //TASK1_CSETGRAPH_H
