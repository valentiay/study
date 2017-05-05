#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <queue>
#include <iomanip>

/******************************************************************************/

using std::vector;
using std::list;
using std::cin;
using std::cout;
using std::pair;
using std::queue;

/******************************************************************************/

class Graph{
public:
    Graph(unsigned int verticesNumber);

    void addEdge(int from, int to);

    int verticesCount() const;

    int countPaths(int from, int to);

    void getNextVertices(int vertex, vector<int> & vertices) const;

    void getPrevVertices(int vertex, vector<int> & vertices) const;

private:
    struct Vertex{
        Vertex();

        int     depth;
        int     pathes;
    };

    unsigned int verticesNumber_;

    vector<list<int>> out_;

    vector<list<int>> in_;
};

/******************************************************************************/

// Finds required diameter
double search(int left, int right, int r, vector<pair<int,int>> cords)
{
    double a = 0;
    double b = right - left;
    int n = cords.size();
    // Binary search
    while(b - a > 5e-5){
        double c = (a + b) / 2;
        Graph graph(n + 2);

        // Building graph
        // Adding edge if table doesn't fit
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                if(sqrt(pow(cords[i].first - cords[j].first, 2)
                + pow(cords[i].second - cords[j].second, 2)) - 2 * r < c)
                    graph.addEdge(i, j);
        for(int i = 0; i < n; i++){
            if(cords[i].first - left - r < c)
                graph.addEdge(i, n);
            if(right - cords[i].first - r < c)
                graph.addEdge(i, n + 1);
        }

        if(graph.countPaths(n, n + 1) != 0)
            b = c;
        else
            a = c;
    }
    return a;
}

/******************************************************************************/

int main()
{
    int left = 0;
    int right = 0;
    cin >> left >> right;
    int r = 0;
    cin >> r;
    unsigned n = 0;
    cin >> n;
    vector<pair<int, int>> cords(n, pair<int,int>(0, 0));
    for(int i = 0; i < n; i++)
        cin >> cords[i].first >> cords[i].second;

    cout << std::fixed << std::setprecision(3) << search(left, right, r, cords);
}

/******************************************************************************/

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
    out_[to].push_back(from);
    in_[from].push_back(to);
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



int Graph::countPaths(int from, int to)
{
    vector<Vertex> verticesStates(verticesNumber_);
    queue<int> verticesQueue;
    verticesQueue.push(from);
    verticesStates[from].pathes = 1;

    while(!verticesQueue.empty()){
        vector<int> nextVertices;
        int currentVertex = verticesQueue.front();
        verticesQueue.pop();
        getNextVertices(currentVertex, nextVertices);

        for(int i : nextVertices){
            if(!verticesStates[i].pathes){
                verticesQueue.push(i);
                verticesStates[i].depth = verticesStates[currentVertex].depth + 1;
                verticesStates[i].pathes = verticesStates[currentVertex].pathes;
            }
            else if(verticesStates[i].depth == verticesStates[currentVertex].depth + 1){
                verticesStates[i].pathes += verticesStates[currentVertex].pathes;
            }
        }
    }
    return verticesStates[to].pathes;
}

/****************************GRAPH::PAIR***************************************/

Graph::Vertex::Vertex():
        pathes(0),
        depth(0)
{}