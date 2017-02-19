#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>

/******************************************************************************/

using std::unordered_set;
using std::vector;
using std::queue;
using std::cin;
using std::cout;

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

    vector<unordered_set<int>>    sets_;

};

/****************************GRAPH*********************************************/

int main() {
    unsigned int n;
    cin >> n;
    Graph graph(n);

    int k;
    cin >> k;
    for(int i = 0; i < k; i++){
        int a, b;
        cin >> a >> b;
        graph.addEdge(a, b);
    }
    std::cout << graph.findMinCycle();
}



Graph::Graph(unsigned int verticesNumber):
        verticesNumber_(verticesNumber),
        sets_(verticesNumber_)
{}



void Graph::addEdge(int from, int to){
    sets_[from].insert(to);
    sets_[to].insert(from);
}



int Graph::verticesCount() const{
    return verticesNumber_;
}



void Graph::getNextVertices(int vertex, vector<int> &vertices) const{
    for(int i : sets_[vertex])
        vertices.push_back(i);
}



int Graph::BFS(int vertex){
    vector<Pair> verticesStates(verticesNumber_);
    queue<int> verticesQueue;
    verticesQueue.push(vertex);
    verticesStates[vertex].visited = true;
    int minCycle = verticesNumber_ + 1;

    while(!verticesQueue.empty()){
        vector<int> nextVertices;
        int currentVertex = verticesQueue.front();
        getNextVertices(currentVertex, nextVertices);

        for(int i : nextVertices){
            int currentCycle =
             verticesStates[currentVertex].depth + verticesStates[i].depth + 1;

            if(!verticesStates[i].visited){
                verticesQueue.push(i);
                verticesStates[i].visited = true;
                verticesStates[i].depth =
                        verticesStates[currentVertex].depth + 1;
                verticesStates[i].father = currentVertex;
            }
            else if(i != verticesStates[currentVertex].father
                    && currentCycle < minCycle){
                minCycle = currentCycle;
            }
        }
        verticesQueue.pop();
    }

    return minCycle;
}



int Graph::findMinCycle(){
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