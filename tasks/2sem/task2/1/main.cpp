#include <iostream>
#include <vector>
#include <set>

/******************************************************************************/

using std::vector;
using std::cin;
using std::cout;
using std::set;
using std::pair;
using std::make_pair;

const unsigned INFINITY = 4294967295;

/******************************************************************************/

class Graph {
public:
    Graph(unsigned verticesNumber);

    void addEdge(unsigned from, unsigned to, unsigned weight);

    void getRelated(unsigned vertex, vector<unsigned> &vertices) const;

    unsigned getWeight(unsigned v, unsigned u) const;

    unsigned getSize() const;

private:
    unsigned size_;
    vector<vector<unsigned>> matrix_;
};

/******************************************************************************/

void dijkstra(const Graph &graph, unsigned from, vector<unsigned> &lens) {
    // Initializing set which is used as priority queue.
    // Pair stores <distance, vertex>
    set<pair<unsigned, unsigned>> q;
    q.insert(make_pair(0, from));

    // Initializing vector storing length of paths
    lens.clear();
    lens.assign(graph.getSize(), INFINITY);
    lens[from] = 0;

    // Initializing vector storing used vertices
    vector<bool> used(graph.getSize(), false);

    // Dijkstra algorithm
    for (int i = 0; i < graph.getSize(); i++) {
        // Stop searching if no reachable vertices left
        if (q.empty())
            break;
        // Extracting vertex from queue
        unsigned v = q.begin()->second;
        q.erase(q.begin());
        used[v] = true;
        // Relaxing related vertices
        vector<unsigned> vertices;
        graph.getRelated(v, vertices);
        for (unsigned u : vertices) {
            if (!used[u] && lens[v] + graph.getWeight(v, u) < lens[u]) {
                q.erase(make_pair(lens[u], u));
                lens[u] = lens[v] + graph.getWeight(v, u);
                q.insert(make_pair(lens[u], u));
            }
        }
    }
}

/******************************************************************************/

int main() {
    // Initializing graph
    unsigned n = 0;
    unsigned m = 0;
    cin >> n >> m;
    Graph graph(n);
    for (unsigned i = 0; i < m; i++) {
        unsigned s = 0;
        unsigned t = 0;
        unsigned w = 0;
        cin >> s >> t >> w;
        graph.addEdge(s, t, w);
    }
    unsigned from = 0;
    unsigned to = 0;
    cin >> from >> to;

    vector<unsigned> lens;
    dijkstra(graph, from, lens);

    if (lens[to] != INFINITY)
        cout << (lens[to]);
    else
        cout << -1;

    return 0;
}

/****************************GRAPH*********************************************/

Graph::Graph(unsigned int size) :
        size_(size),
        matrix_(size_, vector<unsigned>(size_, INFINITY)) {}


void Graph::addEdge(unsigned from, unsigned to, unsigned weight) {
    matrix_[to][from] = matrix_[from][to] = weight;
}


unsigned Graph::getSize() const {
    return size_;
}


void Graph::getRelated(unsigned vertex, vector<unsigned> &vertices) const {
    vertices.clear();
    for (unsigned i = 0; i < size_; i++)
        if (matrix_[vertex][i] < INFINITY)
            vertices.push_back(i);
}


unsigned Graph::getWeight(unsigned v, unsigned u) const {
    return matrix_[v][u];
}