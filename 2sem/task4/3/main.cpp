#include <iostream>
#include <vector>
#include <cmath>

/******************************************************************************/

// Function object for solving LCA problem
class LCA{
public:
    // Builds and pre-processes tree using vector where i-th index stores father of i-th node
    LCA(std::vector<std::vector<unsigned>>& parents);

    // Returns LCA
    unsigned operator()(unsigned a, unsigned b);
    unsigned operator()(std::pair<unsigned, unsigned> query);

private:
    // DFS filling sparse table during pre-processing
    void DFS(unsigned node, unsigned parent);

    // Checks if a is ancestor of b
    bool isAncestor(unsigned a, unsigned b);

    // Timer used in DFS
    unsigned _timer;
    // Number of nodes
    size_t _n;
    // Size of ancestors table
    unsigned _l;

    // Stores children for i-th node
    std::vector<std::vector<unsigned>>& _tree;
    // Time of visiting i-th node
    std::vector<unsigned> _tin;
    // Time of leaving i-th node
    std::vector<unsigned> _tout;
    // Sparse table of ancestors
    std::vector<std::vector<unsigned>> _ancestors;
};

/******************************************************************************/

// Function object for generating next query
class NextQuery{
public:
    NextQuery(unsigned a0,
              unsigned b0,
              unsigned long x,
              unsigned long y,
              unsigned long z,
              unsigned n);

    std::pair<unsigned, unsigned> operator()(unsigned result);

private:
    unsigned _a;
    unsigned _b;
    unsigned long _x;
    unsigned long _y;
    unsigned long _z;
    unsigned _n;
};

/******************************************************************************/

int main() {
    // Initializing vector of parents
    unsigned n = 0;
    unsigned m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<unsigned>> parents(n);
    for (unsigned i = 1; i < n; i++) {
        unsigned x;
        std::cin >> x;
        parents[x].push_back(i);
    }

    // Initializing queries
    unsigned a0 = 0;
    unsigned b0 = 0;
    std::cin >> a0 >> b0;
    unsigned long x = 0;
    unsigned long y = 0;
    unsigned long z = 0;
    std::cin >> x >> y >> z;
    NextQuery nextQuery(a0, b0, x, y, z, n);

    // Generating LCA object
    LCA lca(parents);
    unsigned long ans = 0;
    unsigned result = 0;
    for (unsigned i = 0; i < m; i++) {
        result = lca(nextQuery(result));
        ans += result;
    }
    std::cout << ans;

    return 0;
}

/**************************************************************************************************/

LCA::LCA(std::vector<std::vector<unsigned>>& parents)
        : _tree(parents)
        , _n(parents.size())
        , _l(static_cast<unsigned>(ceil(log2(_n))))
        , _tin(_n)
        , _tout(_n)
        , _ancestors(_n, std::vector<unsigned>(_l + 1))
        , _timer(0)
{
    DFS(0, 0);
}



unsigned LCA::operator()(unsigned a, unsigned b)
{
    if (isAncestor(a, b))
        return a;
    if (isAncestor(b, a))
        return b;
    // Rising up the tree
    for (int i = _l; i >= 0; --i)
        if (!isAncestor(_ancestors[a][i], b))
            a = _ancestors[a][i];
    return _ancestors[a][0];
}



unsigned LCA::operator()(std::pair<unsigned, unsigned> query)
{
    return LCA::operator()(query.first, query.second);
}



void LCA::DFS(unsigned node, unsigned parent)
{
    _tin[node] = _timer++;
    _ancestors[node][0] = parent;
    for (int i = 1; i <= _l; i++)
        _ancestors[node][i] = _ancestors[_ancestors[node][i - 1]][i - 1];
    for (size_t i = 0; i < _tree[node].size(); ++i) {
        unsigned to = _tree[node][i];
        if (to != parent)
            DFS(to, node);
    }
    _tout[node] = _timer++;
}



bool LCA::isAncestor(unsigned a, unsigned b)
{
    return _tin[a] <= _tin[b] && _tout[b] <= _tout[a];
}

/**************************************************************************************************/

NextQuery::NextQuery(unsigned a0,
                     unsigned b0,
                     unsigned long x,
                     unsigned long y,
                     unsigned long z,
                     unsigned n)
        : _a(a0)
        , _b(b0)
        , _x(x)
        , _y(y)
        , _z(z)
        , _n(n)
{}



std::pair<unsigned, unsigned> NextQuery::operator()(unsigned result)
{
    std::pair<unsigned, unsigned> ans((_a + result) % _n, _b);
    _a = (_a * _x + _b * _y + _z) % _n;
    _b = (_b * _x + _a * _y + _z) % _n;
    return ans;
}