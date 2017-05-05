#include "../include/checker.h"

/* quantity  |  approximate time
 *  2..10    |      0 s
 *   11      |      1 s
 *   12      |      15 s
 *   13      |      3 min
 *   14      |     40 min
 *   15      |     9 hours
*/


int main() {
    // !!! Dots coordinates are integral, dispersion is 100 !!!
    vector<unsigned> seeds{42};
    vector<unsigned> quantities{10};
    cout << "Results for 10 points, 10 seeds:\n";
    runTests(9, quantities);
    cout << "\nResults for 2..12 points, 1 seed:\n";
    runTests(seeds, 12);
}