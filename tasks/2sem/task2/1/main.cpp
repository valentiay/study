#include <iostream>
#include <vector>
#include <set>
#include <list>

/******************************************************************************/

using std::vector;
using std::cin;
using std::cout;
using std::set;
using std::pair;
using std::make_pair;
using std::list;

const unsigned INFINITY = 4294967295;

typedef pair<unsigned, unsigned> pair_uu;

/******************************************************************************/

// Undirected graph. List realization
class Graph {
public:
    Graph(unsigned verticesNumber);

    void addEdge(unsigned from, unsigned to, unsigned weight);

    // Vertices - vector of pairs <vertex, weight>
    void getRelated(unsigned vertex, vector<pair_uu> &vertices) const;

    unsigned getSize() const;

private:
    unsigned size_;
    vector<list<pair_uu>> edges_;
};

/******************************************************************************/

int dijkstra(Graph const &graph, int from, int to) {
    // Stores true is [i] vertex was extracted from the queue
    vector<bool> used(graph.getSize(), false);
    // Stores length of path to [i] vertex
    vector<unsigned> path(graph.getSize(), INFINITY);
    path[from] = 0;
    // Priority queue
    set<pair_uu> queue;
    queue.emplace(make_pair(0, from));
    // Dijkstra algorithm
    while (!queue.empty()) {
        // Extracting vertex with minimal path
        unsigned v = (queue.begin())->second;
        queue.erase(queue.begin());
        used[v] = true;

        // Relaxing every related vertex
        vector<pair_uu> paths;
        graph.getRelated(v, paths);
        for (pair_uu c : paths) {
            if (path[c.first] > path[v] + c.second) {
                // Remove old value from queue
                if (path[c.first] != INFINITY)
                    queue.erase(make_pair(path[c.first], c.first));
                // Update path and insert it into queue
                path[c.first] = path[v] + c.second;
                queue.emplace(pair<int, int>(path[c.first], c.first));
            }
        }
    }

    // Return distance to the target vertex or -1 if target is unreachable
    return ((path[to] != INFINITY) ? (static_cast<int>(path[to])) : (-1));
}

/******************************************************************************/

int main() {
    std::ios::sync_with_stdio(false);
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

    // Printing answer
    cout << dijkstra(graph, from, to);

    return 0;
}

/****************************GRAPH*********************************************/

Graph::Graph(unsigned int size) :
        size_(size),
        edges_(size_, list<pair_uu>()) {}


void Graph::addEdge(unsigned from, unsigned to, unsigned weight)
{
    edges_[from].push_back(make_pair(to, weight));
    edges_[to].push_back(make_pair(from, weight));
}


unsigned Graph::getSize() const
{
    return size_;
}


void Graph::getRelated(unsigned vertex, vector<pair_uu> &vertices) const
{
    vertices.clear();
    for (const pair_uu &i : edges_[vertex])
        vertices.push_back(i);
}