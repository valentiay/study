//
// Created by valentiay on 4/22/17.
//

#include "../include/TSP_BF.h"

double MinTourLenBF::operator()(const Graph & graph)
{
    graph_ = &graph;
    n_ = graph_->getSize();

    if(n_ <= 1)
        return 0;

    // Initializing minTour_ with trivial permutation
    tour_.clear();
    for(unsigned i = 1; i < n_; i++)
        tour_.push_back(i);
    minTour_ = getTourLen();

    // Checking all permutations
    permutations(1);

    return minTour_;
}



void MinTourLenBF::permutations(unsigned depth)
{
    // If every vertex is used in tour, check it an return
    if(depth == tour_.size()){
        double tour = getTourLen();
        if(tour < minTour_)
            minTour_ = tour;
        return;
    }

    // Recursively getting permutations
    for(int i = 0; i <= depth; i++){
        // Swap
        unsigned tmp = tour_[i];
        tour_[i] = tour_[depth];
        tour_[depth] = tmp;

        permutations(depth + 1);

        // Swap
        tmp = tour_[i];
        tour_[i] = tour_[depth];
        tour_[depth] = tmp;
    }
}



double MinTourLenBF::getTourLen()
{
    double tour = graph_->getWeight(tour_[n_ - 2], 0)
                  + graph_->getWeight(0, tour_[0]);
    for(unsigned i = 1; i < n_ - 1; i++)
        tour += graph_->getWeight(tour_[i - 1], tour_[i]);
    return tour;
}