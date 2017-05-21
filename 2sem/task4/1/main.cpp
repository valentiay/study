#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

/**************************************************************************************************/

// Function object for finding 2nd statistic on a segment
class R2Q {
public:
    // Takes sequence of int and performs pre-processing
    R2Q(const std::vector<int> & sequence)
        : _sequence(sequence)
        , _n(sequence.size())
        , _sparseTable((size_t)(ceil(log2(_n))),
                       std::vector<std::pair<int*, int*>>(_n, std::pair<int*, int*>(0, 0)))
    {
        // Pre-counting binary logarithms for every index in sequence
        for (size_t i = 0; i < _n + 1; i++)
            _log2.push_back((size_t)ceil(log2(i)));

        // Initializing 1st row of table with sequence elements
        for (size_t i = 0; i < _n; i++) {
            _sparseTable[0][i].first = &_sequence[i];
            _sparseTable[0][i].second = &_sequence[i];
        }

        // Initializing other rows of table with first two least elements of segment
        for (size_t i = 1; i < _sparseTable.size(); i++) {
            for (size_t j = 0; j < _n - (1 << i) + 1; j++) {
                std::pair<int*, int*> mp = getMinPair(_sparseTable[i - 1][j],
                                                      _sparseTable[i - 1][j + (1 << (i - 1))]);
                _sparseTable[i][j].first = mp.first;
                _sparseTable[i][j].second = mp.second;
            }
        }
    }

    // Returns 2nd statistic on segment [a, b]
    int operator()(size_t a, size_t b)
    {
        // Size of required sparse table segment
        size_t k = _log2[b - a + 1] - 1;
        // Finding second least element of two segments
        return *getMinPair(_sparseTable[k][a], _sparseTable[k][b - (1 << k) + 1]).second;
    }

private:
    // Returns pair of two least unique elements for two pairs, where
    // a.first <= a.second and b.first <= b.second
    std::pair<int*, int*> getMinPair(std::pair<int*, int*> a, std::pair<int*, int*> b)
    {
        std::pair<int*, int*> ans;
        // Initializing and sorting vector of elements
        std::vector<int*> order{a.first, a.second, b.first, b.second};
        std::sort(order.begin(), order.end(), [](int* l, int* r){return *l < *r;});

        // Taking first element and distinct min element from the rest ones
        ans.first = order[0];
        for (short i = 1; i < 4; i++)
            if (order[i] != order[0]) {
                ans.second = order[i];
                break;
            }
        return ans;
    }

private:
    // Stores sequence of ints
    std::vector<int> _sequence;
    // Stores size of the sequence
    size_t _n;
    // Stores binary logarithms
    std::vector<size_t> _log2;
    // Stores sparse table
    std::vector<std::vector<std::pair<int*, int*>>> _sparseTable;
};

/**************************************************************************************************/

int main() {
    // Reading sizes
    size_t n = 0;
    size_t m = 0;
    std::cin >> n >> m;

    // Reading sequence of ints
    std::vector<int> sequence(n);
    for (size_t i = 0; i < n; i++) {
        int x = 0;
        std::cin >> x;
        sequence[i] = x;
    }

    // Initializing 2nd statistic finder
    R2Q r2q(sequence);

    // Answering for requests
    for (size_t i = 0; i < m; i++) {
        size_t a = 0;
        size_t b = 0;
        std::cin >> a >> b;
        std::cout << r2q(a - 1, b - 1) << '\n';
    }

    return 0;
}