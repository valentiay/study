//
// Created by valentiay on 4/22/17.
//

#ifndef INC_2_CHECKER_H
#define INC_2_CHECKER_H

#include <iomanip>
#include <cmath>
#include <ctime>
#include "graph.h"
#include "dots.h"
#include "TSP_BF.h"
#include "TSP_MST.h"

using std::cin;
using std::cout;
using std::fixed;
using std::setw;
using std::setprecision;
using std::move;

// Container for test results
struct TestResults{
    TestResults(bool success,
                time_t time,
                double ansBF,
                double ansMST,
                double error);

    // True if test was passed
    bool success;
    // Time of testing in seconds
    time_t time;
    // Brute force answer
    double ansBF;
    // MST answer
    double ansMST;
    // Error
    double error;
};

// Runs test for every seed in seeds and every number of dots in ns
// Criteria is maximal ratio of MST answer to BF answer
TestResults && runTests(const vector<unsigned> & seeds,
                        const vector<unsigned> & ns,
                        double criteria);

// Runs test for every seed in [0..maxSeed]
// and every number of dots im n [2..maxN]
// Criteria is maximal permissible ratio of MST answer to BF answer
TestResults && runTests(unsigned maxSeed = 100,
                        unsigned maxN = 10,
                        double criteria = 2);

TestResults && runTests(const vector<unsigned> & seeds,
                        unsigned maxN,
                        double criteria = 2);

TestResults && runTests(unsigned maxSeed,
                        const vector<unsigned> & ns,
                        double criteria = 2);

// Runs test with specific seed and number of dots (n)
// Criteria is maximal ratio of MST answer to BF answer
TestResults && runTest(unsigned seed, unsigned n, double criteria);

#endif //INC_2_CHECKER_H
