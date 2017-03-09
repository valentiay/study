#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <functional>

#define INFINITY 2147483647

/******************************************************************************/

using std::vector;
using std::list;
using std::priority_queue;
using std::cin;
using std::cout;
using std::function;

/******************************************************************************/

class Graph{
public:
    Graph(unsigned verticesNumber);

    void addEdge(int from, int to, int weight);

    unsigned getSize() const;

    void getRelated(int vertex, vector<int> & vertices) const;

    int getWeight(int v, int u) const;
    
private:

    unsigned int size_;

    vector<vector<int>> matr_;
};

/******************************************************************************/

struct Path{
    Path(int v, int len):
            v(v),
            len(len)
    {}
    int v;
    int len;
};

void dijkstra(const Graph &graph, int from, vector<int> &lens){
    // Initializing priority queue
    function<bool(Path, Path)> compare = [](Path L, Path R)->bool{
        return L.len > R.len;
    };
    priority_queue<Path, vector<Path>, function<bool(Path, Path)>> queue(compare);
    queue.push(Path(from, 0));

    // Initializing vector storing length of paths
    lens.clear();
    lens.assign(graph.getSize(), INFINITY);
    lens[from] = 0;

    // Initializing vector storing used vertices
    vector<bool> used(graph.getSize(), false);

    // Dijkstra algorithm
    for(int i = 0; i < graph.getSize(); i++){
        while(!queue.empty() && used[queue.top().v])
            queue.pop();
        if(queue.empty())
            break;
        int v = queue.top().v;
        queue.pop();
        used[v] = true;
        vector<int> vertices;
        graph.getRelated(v, vertices);
        for(int u : vertices){
            if(!used[u] && lens[v] + graph.getWeight(v, u) < lens[u]){
                lens[u] = lens[v] + graph.getWeight(v, u);
                queue.push(Path(u, lens[u]));
            }
        }
    }
}

/******************************************************************************/

int main() {
    unsigned n = 0;
    unsigned m = 0;
    cin >> n >> m;
    Graph graph(n);
    for(unsigned i = 0; i < m; i++){
        int f = 0;
        int t = 0;
        int w = 0;
        cin >> f >> t >> w;
        graph.addEdge(f, t, w);
    }
    unsigned from = 0;
    unsigned to = 0;
    cin >> from >> to;

    vector<int> lens;
    dijkstra(graph, from, lens);

    if(lens[to] == INFINITY)
        lens[to] = -1;
    cout << lens[to];

    return 0;
}

/****************************GRAPH*********************************************/

Graph::Graph(unsigned int size):
        size_(size),
        matr_(size_, vector<int>(size_, INFINITY))
{}



void Graph::addEdge(int from, int to, int weight)
{
    matr_[to][from] = matr_[from][to] = weight;
}



unsigned Graph::getSize() const
{
    return size_;
}



void Graph::getRelated(int vertex, vector<int> & vertices) const
{
    vertices.clear();
    for(int i = 0; i < size_; i++)
        if(matr_[vertex][i] < INFINITY)
            vertices.push_back(i);
}


int Graph::getWeight(int v, int u) const{
    return matr_[v][u];
}