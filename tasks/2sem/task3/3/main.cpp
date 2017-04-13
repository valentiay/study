#include <iostream>
#include <vector>
#include <cassert>

/******************************************************************************/

using std::cin;
using std::cout;
using std::vector;
using std::pair;
using std::make_pair;
using std::min;

typedef pair<unsigned, unsigned> pair_uu;

const unsigned INFINITY = 4294967295;

/******************************************************************************/

class Graph {
public:
    Graph(unsigned verticesNumber);

    void increaseEdge(unsigned from, unsigned to, int weight);

    // Vertices - vector of pairs <vertex, weight>
    void getNext(unsigned vertex, vector<pair_uu> &vertices) const;

    unsigned getWeight(unsigned from, unsigned to) const;

    unsigned getSize() const;

    Graph & operator=(Graph other);

private:
    unsigned size_;
    vector<vector<unsigned>> matrix_;
};

/******************************************************************************/

class MaxFlow{
public:
    unsigned operator()(const Graph &original, unsigned s, unsigned t) {
        t_ = t;
        original_ = &original;
        flows_ = new Graph(original.getSize());
        unsigned delta = 0;
        unsigned result = 0;
        do{
            used_.assign(original.getSize(), false);
            delta = DFS(s, INFINITY);
            result += delta;
        }while(delta > 0);
        delete flows_;
        return result;
    }

private:
    unsigned DFS(unsigned u, unsigned cMin){
        if (u == t_)
            return cMin;
        used_[u] = true;
        vector<pair_uu> next;
        original_->getNext(u, next);
        for(const pair_uu & v : next){
            if(!used_[v.first] && flows_->getWeight(u, v.first) < original_->getWeight(u, v.first)) {
                unsigned delta = DFS(v.first, min(cMin, original_->getWeight(u, v.first) - flows_->getWeight(u, v.first)));
                if(delta > 0) {
                    flows_->increaseEdge(u, v.first, delta);
                    flows_->increaseEdge(v.first, u, -delta);
                    return delta;
                }
            }
        }
        return 0;
    }
    const Graph * original_;
    Graph * flows_;
    vector<bool> used_;
    int t_;
};

//int dfs(int u, int Cmin):         // Cmin — пропускная способность в текущем подпотоке
//if (u = t)
//return Cmin
//        visited[u] = true
//for (v in u.children)
//int uv = edge(u, v)
//if (not visited[v]) and (uv.f < uv.c)
//int delta = dfs(v, min(Cmin, uv.c - uv.f))
//if (delta > 0)
//uv.f += delta
//        uv.backEdge.f -= delta
//return delta
//return 0

/******************************************************************************/

int main() {
    MaxFlow maxFlow;
    do{
        unsigned n = 0;
        cin >> n;
        if(n == 0)
            break;
        Graph graph(n);
        unsigned s = 0;
        unsigned t = 0;
        unsigned c = 0;
        cin >> s >> t >> c;
        for(int i = 0; i < c; i++){
            unsigned v = 0;
            unsigned u = 0;
            unsigned w = 0;
            cin >> v >> u >> w;
            graph.increaseEdge(v - 1, u - 1, w);
        }
        cout << maxFlow(graph, s, t);
    }while(true);
}

/****************************GRAPH*********************************************/

Graph::Graph(unsigned int size) :
        size_(size),
        matrix_(size_, vector<unsigned>(size_, 0)) {}


void Graph::increaseEdge(unsigned from, unsigned to, int weight)
{
    assert(static_cast<int>(matrix_[from][to]) + weight > 0);
    matrix_[from][to] += weight;
}


unsigned Graph::getSize() const
{
    return size_;
}


unsigned Graph::getWeight(unsigned from, unsigned to) const
{
    return matrix_[from][to];
}


void Graph::getNext(unsigned vertex, vector<pair_uu> &vertices) const
{
    vertices.clear();
    for(int i = 0; i < size_; i++){
        if(matrix_[vertex][i] != 0)
            vertices.push_back(make_pair(i, matrix_[vertex][i]));
    }
}


Graph& Graph::operator=(Graph other)
{
    size_ = other.size_;
    matrix_ = other.matrix_;
    return *this;
}