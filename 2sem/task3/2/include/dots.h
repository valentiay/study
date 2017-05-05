//
// Created by valentiay on 4/21/17.
//

#ifndef INC_2_DOTS_H
#define INC_2_DOTS_H

#include <vector>
#include <cstdlib>
#include <cmath>
#include "graph.h"

using std::pair;
using std::vector;
using std::srand;
using std::rand;
using std::sqrt;
using std::log;
using std::move;

typedef pair<int, int> dot;

// graph is connected graph where vertices are dots with integral coordinates
// generated with Gaussian distribution (dispersion 100, expected value (0,0))
// and edges are distances between this dots
void generateDots(unsigned seed, Graph & graph);

#endif //INC_2_DOTS_H
