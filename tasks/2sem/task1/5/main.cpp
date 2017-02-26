#include <unordered_set>
#include <vector>
#include <list>
#include <stack>
#include <iostream>

/******************************************************************************/

using std::unordered_set;
using std::vector;
using std::list;
using std::stack;
using std::cin;
using std::cout;

/******************************************************************************/

class Graph{
public:
    Graph(unsigned int verticesNumber);

    void addEdge(int from, int to);

    int verticesCount() const;

    void getNextVertices(int vertex, vector<int> & vertices) const;

    void getPrevVertices(int vertex, vector<int> & vertices) const;

private:

    unsigned int verticesNumber_;

    vector<list<int>> out_;

    vector<list<int>> in_;
};

/******************************************************************************/

// Function object. operator() returns condensed graph
class Condenser{
public:
    Graph operator() (Graph * graph);

private:
    // Tarjan algorithm. Finds LCS
    void DFS(int vertex);

    // Returns condensed graph
    Graph condense();
    // Current time for Tarjan algorithm
    int time_;
    // Current component for Tarjan algorithm
    int component_;
    // Graph to be condensed
    Graph * graph_;
    // Number of vertices
    int n_;
    // Stack for Tarjan algorithm
    stack<int> stack_;
    // Stores number of component for each vertex
    vector<int> components_;
    // Stores visited vertices
    vector<bool> visited_;
    // Stores lowest vertex for Tarjan algorithm
    vector<int> low_;
};

/****************************MAIN**********************************************/

int main()
{
    unsigned int n = 0;
    cin >> n;
    Graph graph(n);

    // Building graph
    int k;
    cin >> k;
    for(int i = 0; i < k; i++){
        int a, b;
        cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
    }

    Condenser lcs;
    // Condensed graph
    Graph c = lcs(&graph);

    // Counting vertices without outgoing and ingoing edges
    int begs = 0;
    int ends = 0;
    if(c.verticesCount() > 1)
        for(int i = 0; i < c.verticesCount(); i++){
            vector<int> ver;
            c.getNextVertices(i, ver);
            if(ver.size() == 0) ends++;
            c.getPrevVertices(i, ver);
            if(ver.size() == 0) begs++;
        }

    // Answer is maximum from vertices without outgoing and ingoing edges
    cout << std::max(begs, ends);
}

/****************************GRAPH*********************************************/

Graph::Graph(unsigned int verticesNumber):
        verticesNumber_(verticesNumber),
        out_(verticesNumber_),
        in_(verticesNumber_)
{}



void Graph::addEdge(int from, int to)
{
    // Checking if edge already exists
    for(int i : out_[from])
        if(i == to)
            return;

    out_[from].push_back(to);
    in_[to].push_back(from);
}



int Graph::verticesCount() const
{
    return verticesNumber_;
}



void Graph::getNextVertices(int vertex, vector<int> & vertices) const
{
    vertices.clear();
    for(int i : out_[vertex])
        vertices.push_back(i);
}



void Graph::getPrevVertices(int vertex, vector<int> & vertices) const
{
    vertices.clear();
    for(int i : in_[vertex])
        vertices.push_back(i);
}

/****************************CONDENSER*****************************************/

Graph Condenser::operator()(Graph * graph)
{
    graph_ = graph;
    n_ = graph_->verticesCount();
    visited_.assign(n_, false);
    low_.assign(n_, 0);
    components_.assign(n_, 0);
    time_ = 0;
    component_ = 0;

    // Tarjan algorithm
    for(int i = 0; i < graph->verticesCount(); i++){
        if(!visited_[i])
            DFS(i);
    }

    return condense();
}



void Condenser::DFS(int vertex)
{
    low_[vertex] = time_++;
    visited_[vertex] = true;
    stack_.push(vertex);
    bool isRoot = true;

    vector<int> children;
    graph_->getNextVertices(vertex, children);
    for(int i : children){
        // Recursively launching Tarjan
        if(!visited_[i])
            DFS(i);
        // Fixing lowest value
        if(low_[i] < low_[vertex]){
            low_[vertex] = low_[i];
            isRoot = false;
        }
    }

    // If lowest for vertex is itself, making component
    if(isRoot){
        while(true){
            int x = stack_.top();
            stack_.pop();
            components_[x] = component_;
            low_[x] = graph_->verticesCount() + 1;
            if(vertex == x)
                break;
        }
        component_++;
    }
}



Graph Condenser::condense()
{
    // Visiting every edge
    // If edge connects different components adding it to condensed graph
    Graph condensed(component_);
    for(int v = 0; v < graph_->verticesCount(); v++){
        vector<int> vertices;
        graph_->getNextVertices(v, vertices);
        for(int u : vertices)
            if(components_[u] != components_[v])
                condensed.addEdge(components_[v], components_[u]);
    }
    return condensed;
}