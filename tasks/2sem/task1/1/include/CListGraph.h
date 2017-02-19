//
// Created by valentiay on 16.02.17.
//

#ifndef TASK1_CLISTGRAPH_H
#define TASK1_CLISTGRAPH_H

#include "IGraph.h"
#include <list>

using std::list;

class CListGraph : public IGraph{
public:

                    CListGraph(unsigned int verticesNumber);

                    CListGraph(const IGraph * graph);


    virtual void    AddEdge(int from, int to) override;


    virtual int     VerticesCount() const override;


    virtual void    GetNextVertices(int vertex,
                                    vector<int> & vertices) const override;

    virtual void    GetPrevVertices(int vertex,
                                    vector<int> & vertices) const override;

private:

    unsigned int          verticesNumber_;

    vector<list<int>>     in_;

    vector<list<int>>     out_;

};

#endif //TASK1_CLISTGRAPH_H
