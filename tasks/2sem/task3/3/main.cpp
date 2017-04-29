#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>

/******************************************************************************/

using std::ifstream;
using std::ofstream;
using std::vector;
using std::pair;
using std::make_pair;
using std::min;
using std::queue;

typedef pair<unsigned, unsigned> pair_uu;

const unsigned INFINITY = 2147483647;

/******************************************************************************/

class Graph {
public:
    Graph(unsigned verticesNumber);

    void increaseEdge(unsigned from, unsigned to, int weight);

    // Vertices - vector of pairs <vertex, weight>
    void getRelated(unsigned vertex, vector<pair_uu> &vertices) const;

    unsigned getSize() const;

private:
    unsigned size_;
    vector<vector<unsigned>> matrix_;
};

/******************************************************************************/

// Function object for counting maximal flow in network
class MaxFlow{
public:
    MaxFlow():
            net_(0){}

    // Returns maxiamal flow
    unsigned operator()(const Graph & graph, unsigned s, unsigned t){
        // Edmonds-Karp algorithm
        // Residual network
        net_ = graph;
        // Target vertex
        t_ = t;
        // Delta in flow in current iteration
        unsigned delta = 0;
        // Final flow
        unsigned flow = 0;

        // Increasing flow iteratively
        do{
            delta = IncreaseFlow(s);
            flow += delta;
        }while(delta != 0);

        return flow;
    }

private:
    // Finds path and decreases throughput in residual network
    unsigned IncreaseFlow(unsigned s){
        // Stores pair <parent vertex, weight of edge from parent>
        vector<pair_uu> parents(net_.getSize());
        for(unsigned i = 0; i < net_.getSize(); i++){
            parents[i].first = INFINITY;
            parents[i].second = INFINITY;
        }
        parents[s].first = s;
        parents[s].second = 0;

        // Finding path with BFS
        queue<unsigned> q;
        q.emplace(s);
        while(!q.empty()){
            unsigned v = q.front();
            q.pop();
            if(v == t_)
                break;
            vector<pair_uu> related;
            net_.getRelated(v, related);
            for(pair_uu & edge : related){
                if(parents[edge.first].first == INFINITY){
                    parents[edge.first].first = v;
                    parents[edge.first].second = edge.second;
                    q.push(edge.first);
                }
            }
        }

        // If target vertex isn't reachable delta is 0
        if(parents[t_].first == INFINITY)
            return 0;

        // Finding minimal edge throughput
        unsigned delta = INFINITY;
        unsigned u = t_;
        while(u != s){
            if(parents[u].second < delta)
                delta = parents[u].second;
            u = parents[u].first;
        }

        // Decreasing throughputs in residual network
        u = t_;
        while(u != s){
            net_.increaseEdge(parents[u].first, u, -delta);
            net_.increaseEdge(u, parents[u].first, delta);
            u = parents[u].first;
        }

        return delta;
    }

    unsigned t_;
    Graph net_;
};

/******************************************************************************/

int main() {
    ifstream fin("maxflow.in");
    ofstream fout("maxflow.out");

    // Initializing graph
    unsigned n = 0;
    unsigned m = 0;
    fin >> n >> m;
    Graph graph(n);
    for(int i = 0; i < m; i++){
        unsigned v = 0;
        unsigned u = 0;
        unsigned w = 0;
        fin >> v >> u >> w;
        graph.increaseEdge(v - 1, u - 1, w);
    }

    // Counting maximal flow
    MaxFlow maxFlow;
    fout << maxFlow(graph, 0, n - 1) << '\n';

    fin.close();
    fout.close();
}

/****************************GRAPH*********************************************/

Graph::Graph(unsigned int size) :
        size_(size),
        matrix_(size_, vector<unsigned>(size_, 0)) {}



void Graph::increaseEdge(unsigned from, unsigned to, int weight)
{
    assert(static_cast<int>(matrix_[from][to]) + weight >= 0);
    matrix_[from][to] += weight;
}



unsigned Graph::getSize() const
{
    return size_;
}



void Graph::getRelated(unsigned vertex, vector<pair_uu> &vertices) const
{
    vertices.clear();
    for(unsigned i = 0; i < size_; i++){
        if(matrix_[vertex][i] != 0)
            vertices.push_back(make_pair(i, matrix_[vertex][i]));
    }
}