#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

/******************************************************************************/

// Segment tree with operations of taking maximum and increasing a segment
class SegmentTree{
public:
    // Constructs the tree
    SegmentTree(const std::vector<unsigned>& sequence);

    // Returns max on the segment [a, b]
    unsigned getSegmentMax(size_t a, size_t b);
    // Increases segment [a, b] by delta
    void increaseSegment(size_t a, size_t b, unsigned delta);

private:
    // Recursive function for taking max on a segment
    // node - root of current subtree,
    // left - left bound of current subtree in sequence,
    // right - right bound of current subtree
    // a - left bound of segment to increase in current subtree
    // b - right bound of segment to increase in current subtree
    unsigned getSegmentMax(size_t node, size_t left, size_t right, size_t a, size_t b);
    // Recursive function for increasing segment by delta
    // Arguments same as for max
    void increaseSegment(size_t node, size_t left, size_t right, size_t a, size_t b, unsigned delta);

private:
    // Buffer storing the tree like a heap
    // Tree is stored as a binary tree with full lower level. All unused leafs are initialized with 0
    std::vector<std::pair<unsigned, unsigned>> _tree;
    // Number of tree leafs
    const size_t _n;
};

/******************************************************************************/

int main() {
    // Initializing ticket array
    size_t n = 0;
    std::cin >> n;
    std::vector<unsigned> tickets(n - 1);
    for (int i = 0; i < n - 1; i++) {
        std::cin >> tickets[i];
    }

    // Building tree
    SegmentTree segmentTree(tickets);

    // Getting train capacity
    unsigned capacity = 0;
    std::cin >> capacity;

    // Processing requests
    size_t m = 0;
    std::cin >> m;
    for (size_t i = 0; i < m; i++) {
        size_t a = 0;
        size_t b = 0;
        unsigned num = 0;
        std::cin >> a >> b >> num;
        // If number of tickets is more than train capacity request is impossible
        // Increasing number of tickets otherwise
        if (segmentTree.getSegmentMax(a, b - 1) + num > capacity)
            std::cout << i << ' ';
        else
            segmentTree.increaseSegment(a, b - 1, num);
    }
    return 0;
}

/******************************************************************************/

SegmentTree::SegmentTree(const std::vector<unsigned>& sequence)
        : _n(1ull << (size_t)ceil(log2(sequence.size())))
{
    // Reserving memory for storing tree
    // _tree[i].first - current minimum in relevant segment
    // _tree[i].second - number to increase relevant segment
    _tree.assign(2 * _n - 1, std::pair<unsigned, unsigned>(0, 0));
    // Initialising leafs with sequence members
    for (size_t i = 0; i < sequence.size(); i++)
        _tree[_tree.size() / 2 + i].first = sequence[i];
    // Counting min for every other node
    for (long i = _tree.size() / 2 - 1; i >= 0; i--)
        _tree[i].first = std::max(_tree[2*i + 1].first, _tree[2*i + 2].first);
}



unsigned SegmentTree::getSegmentMax(size_t a, size_t b)
{
    return getSegmentMax(0, 0, _n - 1, a, b);
}



unsigned SegmentTree::getSegmentMax(size_t node, size_t left, size_t right, size_t a, size_t b)
{
    // If DFS is in obviously wrong place, stop searching
    if (a > b || left > right)
        return 0;
    // If relevant segment is included in segment to find max, return current max
    if (a == left && b == right)
        return _tree[node].first + _tree[node].second;

    // Visit child nodes
    size_t med = (left + right) / 2;
    unsigned leftMax = getSegmentMax(2 * node + 1, left, med, a, std::min(b, med));
    unsigned rightMax = getSegmentMax(2 * node + 2, med + 1, right, std::max(a, med + 1), b);
    // Return max increased by subtree delta
    return std::max(rightMax, leftMax) + _tree[node].second;
}



void SegmentTree::increaseSegment(size_t a, size_t b, unsigned delta)
{
    increaseSegment(0, 0, _n - 1, a, b, delta);
}



void SegmentTree::increaseSegment(size_t node,
                                  size_t left,
                                  size_t right,
                                  size_t a,
                                  size_t b,
                                  unsigned delta)
{
    // If DFS is in obviously wrong place, stop searching
    if (a > b || left > right)
        return;
    // If relevant segment is included in segment to increase, increase
    if (a == left && b == right) {
        _tree[node].second += delta;
        return;
    }

    // Visit child nodes
    size_t med = (left + right) / 2;
    increaseSegment(2 * node + 1, left, med, a, std::min(b, med), delta);
    increaseSegment(2 * node + 2, med + 1, right, std::max(a, med + 1), b, delta);
    // Fix max
    _tree[node].first = std::max(_tree[2*node + 1].first + _tree[2*node + 1].second,
                                 _tree[2*node + 2].first + _tree[2*node + 2].second);
}