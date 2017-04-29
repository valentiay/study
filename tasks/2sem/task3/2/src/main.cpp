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
    // Insert seeds to be tested here
    vector<unsigned> seeds{42};
    // Quantities of dots
    vector<unsigned> quantities{13};

    runTests(seeds, quantities, 2);
}