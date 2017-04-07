#include <iostream>
#include <vector>

/******************************************************************************/

using std::vector;
using std::cin;
using std::cout;

/******************************************************************************/

class Graph {
public:
    Graph(unsigned verticesNumber);

    void addEdge(int from, int to, double weight);

    unsigned getSize() const;

    void getNext(int vertex, vector<int> &vertices) const;
    void getPrev(int vertex, vector<int> &vertices) const;

    double getWeight(int v, int u) const;

private:
    unsigned int size_;
    vector<vector<double>> matr_;
};

/******************************************************************************/

// Returns true if arbitrage exists
// T = O(|E||V|^2) (O(|V|^4) for matrix graph
// Bellman-Ford algorithm finds only negative cycles
// which may be reached from start vertex,
// so it is launched for every vertex in the graph
bool arbitrage(const Graph &graph) {
    // Launching B.-F. algorithm for every vertex
    for (int k = 0; k < graph.getSize(); k++) {
        // Initializing vector storing cost of the [i]s currency
        // in initial currency
        vector<double> d(graph.getSize(), -1.0);
        // Initial currency
        d[k] = 1.0;
        vector<int> vertices(graph.getSize());
        // Relaxing vertices |V| - 1 time
        for (int i = 0; i < graph.getSize() - 1; i++) {
            for (int v = 0; v < graph.getSize(); v++) {
                graph.getPrev(v, vertices);
                for (int u : vertices)
                    if (d[u] != -1.0 && d[v] < d[u] * graph.getWeight(u, v))
                        d[v] = d[u] * graph.getWeight(u, v);
            }
        }
        // Checking for negative cycles
        for (int v = 0; v < graph.getSize(); v++) {
            graph.getPrev(v, vertices);
            for (int u : vertices)
                if (d[u] != -1.0 && d[v] < d[u] * graph.getWeight(u, v))
                    return true; // Return true if found
        }
    }
    // Return false if cycle was not found
    return false;
}

/******************************************************************************/

int main() {
    // Initialising graph
    unsigned n = 0;
    cin >> n;
    Graph graph(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            double x;
            cin >> x;
            graph.addEdge(i, j, x);
        }
    }
    // Finding arbitrage
    cout << ((arbitrage(graph))?"YES":"NO");
    return 0;
}

/****************************GRAPH*********************************************/

Graph::Graph(unsigned int size) :
        size_(size),
        matr_(size_, vector<double>(size_, -1)) {}


void Graph::addEdge(int from, int to, double weight)
{
    matr_[from][to] = weight;
}


unsigned Graph::getSize() const
{
    return size_;
}


void Graph::getNext(int vertex, vector<int> &vertices) const
{
    vertices.clear();
    for (int i = 0; i < size_; i++)
        if (matr_[vertex][i] != -1)
            vertices.push_back(i);
}


void Graph::getPrev(int vertex, vector<int> &vertices) const
{
    vertices.clear();
    for (int i = 0; i < size_; i++)
        if (matr_[i][vertex] != -1)
            vertices.push_back(i);
}


double Graph::getWeight(int v, int u) const
{
    return matr_[v][u];
}