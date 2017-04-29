//
// Created by valentiay on 4/23/17.
//

#include "../include/TSP_MST.h"

double MinTourLenMST::operator()(const Graph & graph)
{
    n_ = graph.getSize();

    //Finding MST
    Graph mst(n_);
    MST(graph, mst);

    // Finding tour
    used_.assign(n_, false);
    used_[0] = true;
    DFS(mst, 0);

    // Finding tour length
    double tour = graph.getWeight(tour_[n_ - 2], 0)
                  + graph.getWeight(0, tour_[0]);
    for(unsigned i = 1; i < n_ - 1; i++)
        tour += graph.getWeight(tour_[i - 1], tour_[i]);

    return tour;
}



void MinTourLenMST::DFS(const Graph & mst, unsigned dot)
{
    // Pre-order DFS
    vector<pair_ud> related;
    mst.getRelated(dot, related);
    for(const pair_ud & ud : related){
        if(!used_[ud.first]){
            tour_.push_back(ud.first);
            used_[ud.first] = true;
            DFS(mst, ud.first);
        }
    }
}