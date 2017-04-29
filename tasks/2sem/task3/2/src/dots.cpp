//
// Created by valentiay on 4/21/17.
//

#include "../include/dots.h"

void generateDots(unsigned seed, Graph & graph)
{
    // Stores coordinates
    vector<dot> dots;
    srand(seed);
    for(unsigned i = 0; i < graph.getSize(); i++){
        // Box muller method
        double u = rand()%2001/1000.0 - 1.0;
        double v = sqrt(1 - u*u)*(rand()%2001/1000.0 - 1.0);
        double s = u*u + v*v;
        double p = sqrt(-2*log(s)/s);
        // Making double precision value null if it's too small,
        // because in this case p is NaN
        if(fabs(s - 1) < 0.00001)
            p = 0;
        double x = 100*u*p;
        double y = 100*v*p;
        dots.push_back(dot(x, y));
        // Counting edges
        for(unsigned j = 0; j < i; j++){
            graph.addEdge(i, j, sqrt(pow(x - dots[j].first, 2.0)
                                     + pow(y - dots[j].second, 2.0)));
        }
    }
}