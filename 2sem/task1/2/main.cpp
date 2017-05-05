#include <unordered_set>
#include <vector>
#include <queue>

/******************************************************************************/

using std::unordered_set;
using std::vector;
using std::queue;

/******************************************************************************/

class Graph{
public:

    Graph(unsigned int verticesNumber);


    void    addEdge(int from, int to);


    int     verticesCount() const;


    void    getNextVertices(int vertex,
                            vector<int> & vertices) const;


    int     findMinCycle();

private:

    struct Pair{

        Pair();


        int     depth;

        int     father;

        bool    visited;

    };


    int     BFS(int vertex);


    unsigned int                  verticesNumber_;

    vector<unordered_set<int>>    out_;

};

/****************************GRAPH*********************************************/

int main()
{
    unsigned int n = 0;
    scanf("%d", &n);
    Graph graph(n);

    int k;
    scanf("%d", &k);
    for(int i = 0; i < k; i++){
        int a, b;
        scanf("%d%d", &a, &b);
        graph.addEdge(a, b);
    }
    int minCycle = graph.findMinCycle();
    if(minCycle == graph.verticesCount() + 1)
        minCycle = -1;
    printf("%d", minCycle);
}



Graph::Graph(unsigned int verticesNumber):
        verticesNumber_(verticesNumber),
        out_(verticesNumber_)
{}



void Graph::addEdge(int from, int to)
{
    out_[from].insert(to);
    out_[to].insert(from);
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



int Graph::BFS(int vertex)
{
    vector<Pair> verticesStates(verticesNumber_);
    queue<int> verticesQueue;
    verticesQueue.push(vertex);
    verticesStates[vertex].visited = true;

    while(!verticesQueue.empty()){
        vector<int> nextVertices;
        int currentVertex = verticesQueue.front();
        verticesQueue.pop();
        getNextVertices(currentVertex, nextVertices);

        for(int i : nextVertices){
            if(!verticesStates[i].visited){
                verticesQueue.push(i);
                verticesStates[i].visited = true;
                verticesStates[i].depth = verticesStates[currentVertex].depth + 1;
                verticesStates[i].father = currentVertex;
            }
            else if(i != verticesStates[currentVertex].father){
                return verticesStates[currentVertex].depth + verticesStates[i].depth + 1;
            }
        }
    }
    return verticesNumber_ + 1;
}



int Graph::findMinCycle()
{
    int minCycle = verticesNumber_ + 1;
    for(int i = 0; i < verticesNumber_; i++){
        int cycle = BFS(i);
        if(cycle < minCycle)
            minCycle = cycle;
    }
    return minCycle;
}

/****************************GRAPH::PAIR***************************************/

Graph::Pair::Pair():
        depth(0),
        visited(false)
{}