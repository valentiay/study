#include <unordered_set>
#include <vector>
#include <queue>
#include <list>
#include <iostream>

/******************************************************************************/

using std::unordered_set;
using std::vector;
using std::queue;
using std::list;
using std::cin;
using std::cout;

/******************************************************************************/

class Graph{
public:
    Graph(unsigned int verticesNumber);

    void addEdge(int from, int to);

    int verticesCount() const;

    void getNextVertices(int vertex, vector<int> & vertices) const;

    // Counts paths between vertices
    int countPaths(int from, int to);

private:
    // Stores info about visited vertex. Needed for countPaths()
    struct Vertex{
        Vertex();

        // Vertex' depth in BFS
        int depth;
        // Number of paths from first vertex
        int paths;
    };

    unsigned int verticesNumber_;

    // Outgoing vertices
    vector<list<int>> out_;
};

/****************************GRAPH*********************************************/

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
        graph.addEdge(a, b);
    }

    int from = 0;
    int to = 0;
    cin >> from >> to;

    cout << graph.countPaths(from, to);
}



Graph::Graph(unsigned int verticesNumber):
        verticesNumber_(verticesNumber),
        out_(verticesNumber_)
{}



void Graph::addEdge(int from, int to)
{
    // Checking if edge already exists
        for(int i : out_[from])
            if(i == to)
                return;

    out_[to].push_back(from);
    out_[from].push_back(to);
}



int Graph::verticesCount() const
{
    return verticesNumber_;
}



void Graph::getNextVertices(int vertex, vector<int> & vertices) const
{
    for(int i : out_[vertex])
        vertices.push_back(i);
}



int Graph::countPaths(int from, int to)
{
    // Stores info about visited vertex
    vector<Vertex> states(verticesNumber_);
    // BFS queue
    queue<int> q;
    q.push(from);
    states[from].paths = 1;

    // BFS from first vertex
    while(!q.empty()){
        vector<int> nextVertices;
        // Current vertex
        int v = q.front();
        q.pop();
        getNextVertices(v, nextVertices);

        for(int i : nextVertices){
            if(states[i].paths == 0){
                // If not visited push into queue
                q.push(i);
                states[i].depth = states[v].depth + 1;
                states[i].paths = states[v].paths;
            }
            else if(states[i].depth == states[v].depth + 1){
                // If visited and has same depth increase path number
                states[i].paths += states[v].paths;
            }
        }
    }
    return states[to].paths;
}

/****************************GRAPH::PAIR***************************************/

Graph::Vertex::Vertex():
        paths(0),
        depth(0)
{}