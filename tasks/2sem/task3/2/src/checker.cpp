//
// Created by valentiay on 4/23/17.
//

#include "../include/checker.h"

TestResults::TestResults(bool success,
                         time_t time,
                         double ansBF,
                         double ansMST,
                         double error):
    success(success),
    time(time),
    ansBF(ansBF),
    ansMST(ansMST),
    error(error) {}



TestResults && runTests(const vector<unsigned> & seeds,
                        const vector<unsigned> & ns,
                        double criteria)
{
    // Number of successful tests
    unsigned s = 0;
    // Number of failed tests
    unsigned f = 0;
    // Total time
    time_t timer = 0;
    // Avg MST answer
    double avgMST = 0;
    // Avg BF answer
    double avgBF = 0;
    // Standard deviation
    double stDeviation = 0;
    // Running test for every seed and number of dots
    for(unsigned seed : seeds){
        for(unsigned n : ns) {
            TestResults results = runTest(seed, n, criteria);
            if(results.success)
                s++;
            else
                f++;
            timer += results.time;
            avgMST += results.ansMST;
            avgBF += results.ansBF;
            stDeviation += results.error;
        }
    }
    avgMST /= f + s;
    avgBF /= f + s;
    stDeviation = sqrt(stDeviation) / (f + s);
    // Printing summary
    cout << setw(42) <<"============Testing summary============\n= "
         << setw(8) << s << setw(32) << " tests passed =\n= "
         << setw(8) << f << setw(32) << " tests failed =\n= "
         << setw(8) << s + f << setw(32) << " tests total =\n= "
         << setw(8) << timer + 1 << setw(32) << " ttl time =\n= "
         << setw(8) << std::fixed << setprecision(3)
         << (float)(timer + 1) / (s + f) << setw(32) << " avg time =\n= "
         << setw(8) << avgBF << setw(32) << " AVG BF answer =\n= "
         << setw(8) << avgMST << setw(32) << " AVG MST answer =\n= "
         << setw(8) << stDeviation << setw(32) << "  Standard deviation =\n= "
         << setw(8) << stDeviation / 100 << setw(30) << "Normalized st. deviation =\n";

    if (f == 0)
        cout << setw(40) << "----------Testing  successful----------\n";
    else
        cout << setw(40) << "------------Testing  failed------------\n";

    return move(TestResults(f == 0, timer, avgBF, avgMST, stDeviation));
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



TestResults && runTests(const vector<unsigned> & seeds,
                        unsigned maxN,
                        double criteria)
{
    vector<unsigned> ns;
    for(unsigned i = 2; i <= maxN; i++)
        ns.push_back(i);
    return runTests(seeds, ns, criteria);
}



TestResults && runTests(unsigned maxSeed,
                        const vector<unsigned> & ns,
                        double criteria)
{
    vector<unsigned> seeds;
    for(unsigned i = 0; i <= maxSeed; i++)
        seeds.push_back(i);
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
         << ", ERR: " << fabs(mst - bf)
         << " // SEED: " << seed
         << ", N: " << n << "\n\n";

    return move(TestResults(1 <= mst / bf && mst / bf <= criteria,
                            timer, bf, mst, fabs(mst - bf)));
}
