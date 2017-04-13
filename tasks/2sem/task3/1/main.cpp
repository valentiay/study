#include <vector>
#include <fstream>
#include <set>
#include <list>

/******************************************************************************/

using std::vector;
using std::list;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::make_pair;
using std::set;
using std::swap;

const unsigned INFINITY = 4294967295;

typedef pair<unsigned, unsigned> pair_uu;

/******************************************************************************/

// Undirected pseudograph. List realization
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

unsigned MSTWeight(const Graph & graph){
    // Stores weight of minimal edge from used to unused for [i] vertex
    vector<unsigned> key(graph.getSize(), INFINITY);
    // Stores true if [i] vertex was used in MST
    vector<bool> used(graph.getSize(), false);
    // Priority queue
    set<pair_uu> queue;

    // Initializing keys
    key[0] = 0;
    for(int i = 0; i < graph.getSize(); i++)
        queue.emplace(make_pair(key[i], i));

    // Prim's algorithm
    while(!queue.empty()){
        // Extracting minimum
        unsigned v = queue.begin()->second;
        queue.erase(queue.begin());
        used[v] = true;
        // Relaxing related vertices
        vector<pair_uu> related;
        graph.getRelated(v, related);
        for(const pair_uu & u : related){
            if(!used[u.first] && key[u.first] > u.second){
                // Decreasing key
                queue.erase(queue.find(make_pair(key[u.first], u.first)));
                key[u.first] = u.second;
                queue.emplace(key[u.first], u.first);
            }
        }
    }

    // Recovering answer. Key stores weights of edges in MST
    unsigned ans = 0;
    for(unsigned w : key){
        ans += w;
    }
    return ans;
}

/******************************************************************************/

int main() {
    ifstream fin("kruskal.in");
    ofstream fout("kruskal.out");

    // Initializing graph
    unsigned n = 0;
    unsigned m = 0;
    fin >> n >> m;
    Graph graph(n);
    for(unsigned i = 0; i < m; i++){
        unsigned a = 0;
        unsigned b = 0;
        unsigned w = 0;
        fin >> a >> b >> w;
        graph.addEdge(a - 1, b - 1, w);
    }
    // Printing answer
    fout << MSTWeight(graph);

    fin.close();
    fout.close();
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


void Graph::getRelated(unsigned vertex,
                       vector<pair_uu> &vertices) const
{
    vertices.clear();
    for(const pair_uu & i : edges_[vertex])
        vertices.push_back(i);
}