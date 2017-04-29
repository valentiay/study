//
// Created by valentiay on 4/23/17.
//

#include "../include/checker.h"

TestResults::TestResults(bool success, time_t time, double error):
    success(success),
    time(time),
    error(error){}



TestResults && runTests(vector<unsigned> seeds,
                        vector<unsigned> ns,
                        double criteria)
{
    // Number of successful tests
    unsigned s = 0;
    // Number of failed tests
    unsigned f = 0;
    // Total time
    time_t timer = 0;
    // Maximal error
    double maxErr = 0;
    // Running test for every seed and number of dots
    for(unsigned seed : seeds){
        for(unsigned n : ns) {
            TestResults results = runTest(seed, n, criteria);
            if(results.success)
                s++;
            else
                f++;
            timer += results.time;
            if(results.error > maxErr)
                maxErr = results.error;
        }
    }
    // Printing summary
    cout << "====Testing summary====\n= "
         << setw(7) << s << " tests passed=\n= "
         << setw(7) << f << " tests failed=\n= "
         << setw(7) << s + f << " tests total =\n= "
         << setw(7) << timer + 1 << " s ttl time  =\n= "
         << setw(7) << std::fixed << setprecision(3)
         << (float)(timer + 1) / (s + f) << " s avg time  =\n= "
         << setw(7) << maxErr << " % max error =\n";

    if (f == 0)
        cout << "--Testing  successful--\n";
    else
        cout << "----Testing  failed----\n";

    return move(TestResults(f == 0, timer, maxErr));
}



TestResults && runTests(unsigned maxSeed,
                        unsigned maxN,
                        double criteria)
{
    vector<unsigned> seeds;
    for(unsigned i = 0; i <= maxSeed; i++)
        seeds.push_back(i);
    vector<unsigned> ns;
    for(unsigned i = 2; i <= maxN; i++)
        ns.push_back(i);
    return runTests(seeds, ns, criteria);
}



TestResults && runTest(unsigned seed, unsigned n, double criteria)
{
    // Initializing graph
    Graph graph(n);
    generateDots(seed, graph);

    // Running test
    time_t timer = -time(NULL);
    MinTourLenBF minTourLenBF;
    MinTourLenMST minTourLenMST;
    double bf = minTourLenBF(graph);
    double mst = minTourLenMST(graph);
    timer += time(NULL);

    // Printing test results
    if(1 <= mst / bf && mst / bf <= criteria)
        cout << "[SUCCESS]";
    else
        cout << "[FAILURE]";
    double err = fabs(mst - bf)/bf * 100;
    cout << std::fixed << setprecision(2)
         << " BF Answer: " << bf << " <-> MST Answer: " << mst
         << "\n[ " << (int)((100 - err >= 0)?(100 - err):(0)) << "% ]"
         << "TIME: " << timer
         << ", ERR: " << (int)(fabs(mst - bf)/bf * 100) << "%"
         << " // SEED: " << seed
         << ", N: " << n << "\n\n";

    return move(TestResults(1 <= mst / bf && mst / bf <= criteria,
            timer, (fabs(mst - bf)/bf * 100)));
}
