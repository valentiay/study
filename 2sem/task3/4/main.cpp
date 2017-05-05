#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <queue>
#include <climits>
#include <functional>

/******************************************************************************/

// Structure stored in edges matrix
struct Edge {
    int capacity;
    int flow;
};

// Is used for storing relation - question <-> vertex
enum StringType {
    None = -1, // Is not defined yet
    Pattern, // Question in pattern
    Str, // Question in str
    Special // 1 or 0 vertex
};

// Is used for storing relation vertex -> question
struct VertexInfo {
    StringType type;
    int index;
};

// Stores network used to find hamming distance
struct Network {
    Network();

    // Increases capacity by 1 for edges from -> to and to -> from
    void increaseCapacity(int from, int to);

    // Matrix of edges
    std::vector<std::vector<Edge>> matrix;

    unsigned hammingDistance;
    unsigned numberOfVertices;
    unsigned source;
    unsigned drain;

    std::string pattern;
    std::string str;

    // Stores relation vertex->question
    std::vector<std::vector<int>> vertexToQuestion;
    // Stores relation question->vertex (0 for question in pattern, 1 for str)
    std::vector<VertexInfo> questionToVertex;
};

/******************************************************************************/

// Finds and sets maximal flow in network using Dinic's algorithm
unsigned FindMaxFlow(Network& net);

// Used in FindMaxFlow. Finds shortest way from source tp each vertex in net
// Returns true if drain is reachable from source.
// Shortest ways are stored in distances vector
bool FindMaxFlowBFS(Network& net, std::vector<int>& distances);

// Used in FindMaxFlow. Finds Blocking flow in network.
// Returns value of found flow
// distances - shortest paths to every vertex from source
// (v, p[v]) - first edge for v from which drain can be reached
// v - current vertex
// minC - max flow in complementary network
int FindMaxFlowDFS(Network& net, std::vector<int>& distances, std::vector<int>& p, int v, int minC);

/******************************************************************************/

// Returns minimal Hamming distance and fills str and pattern optimally
unsigned hammingDistance(std::string& str, std::string& pattern);

// Builds networ for finding Hamming distance
void buildNetwork(Network &net);

// Finds vertices reachable from zero vertex using dfs
// and fills questions in str and pattern
void FindZeros(Network &net, int v, std::vector<int> &visited);

/******************************************************************************/

int main()
{
    std::string pattern = "", str = "";
    std::cin >> str;
    std::cin >> pattern;


    std::cout << hammingDistance(str, pattern) << std::endl;
    std::cout << str << std::endl;
    std::cout << pattern << std::endl;

    return 0;
}

/******************************************************************************/

Network::Network() :
        numberOfVertices(2),
        source(0),
        drain(1),
        matrix(2007, std::vector<Edge>(2007, {0, 0})),
        vertexToQuestion(2, std::vector<int>(2007, -1)),
        questionToVertex(2007, {StringType::None, -1}),
        pattern(""),
        str("")
{
    questionToVertex[0] = {StringType::Special, 2};
    questionToVertex[1] = {StringType::Special, 2};
}



void Network::increaseCapacity(int from, int to)
{
    matrix[from][to].capacity += 1;
    matrix[to][from].capacity += 1;
}

/******************************************************************************/

unsigned FindMaxFlow(Network& net)
{
    unsigned maxFlow = 0;
    std::vector<int> distances(net.numberOfVertices);
    std::vector<int> p(net.numberOfVertices);

    // While drain is reachable increasing flow
    while (FindMaxFlowBFS(net, distances)) {
        // Initializing array of pointers (see FindMaxFlow DFS declaration)
        for (int position = 0; position < net.numberOfVertices; position++)
            p[position] = 0;

        // Increasing flow
        int currentFlow;
        do {
            currentFlow = FindMaxFlowDFS(net, distances, p, net.source, INT_MAX);
            maxFlow += currentFlow;
        } while (currentFlow != 0);
    }

    return maxFlow;
}


bool FindMaxFlowBFS(Network& net, std::vector<int>& distances)
{
    // Finding shortest paths from source to every vertex using BFS
    for (int i = 0; i < net.numberOfVertices; i++)
        distances[i] = -1;

    std::queue<unsigned> verticesQueue;
    verticesQueue.push(net.source);
    distances[net.source] = 0;

    while (!verticesQueue.empty()) {
        unsigned v = verticesQueue.front();
        verticesQueue.pop();

        for (unsigned u = 0; u < net.numberOfVertices; u++) {
            unsigned vertex = u;
            if (distances[vertex] == -1 &&
                net.matrix[v][u].flow < net.matrix[v][u].capacity) {
                distances[vertex] = distances[v] + 1;
                verticesQueue.push(vertex);
            }
        }
    }

    // Return true if drain is reachable
    return (distances[net.drain] != -1);
}



int FindMaxFlowDFS(Network& net, std::vector<int>& distances, std::vector<int>& p, int v, int minC)
{
    if (minC == 0 || v == net.drain)
        return minC;

    for (int u = p[v]; u < net.numberOfVertices; u++) {
        p[v]++;

        // If it is impossible to increase flow stop search in this direction
        if (net.matrix[v][u].capacity <= net.matrix[v][u].flow)
            continue;

        // If u is related with v continue search
        if (distances[u] == distances[v] + 1) {
            int newMinC = std::min(minC, net.matrix[v][u].capacity - net.matrix[v][u].flow);
            int delta = FindMaxFlowDFS(net, distances, p, u, newMinC);
            if (delta > 0) {
                // Increasing flow in found path
                net.matrix[v][u].flow += delta;
                net.matrix[u][v].flow -= delta;
                return delta;
            }
        }
    }

    return 0;
}

/******************************************************************************/

unsigned hammingDistance(std::string& str, std::string& pattern)
{
    // Initializing network
    Network net;
    net.hammingDistance = 0;
    net.pattern = pattern;
    net.str = str;
    buildNetwork(net);

    // Increase Hamming distance by maximal flow in built network and
    // split graph into two pieces
    net.hammingDistance += FindMaxFlow(net);

    // Find vertices reachable from zero vertex in
    // residual network and fill questions
    std::vector<int> visited(net.numberOfVertices, 0);
    FindZeros(net, 0, visited);
    // Fill other questions with ones
    for (int i = 0; i < net.pattern.length(); i++)
        if (net.pattern[i] == '?')
            net.pattern[i] = '1';
    for (int i = 0; i < net.str.length(); i++)
        if (net.str[i] == '?')
            net.str[i] = '1';

    str = net.str;
    pattern = net.pattern;
    return net.hammingDistance;
}



void FindZeros(Network& net, int v, std::vector<int>& visited)
{
    // DFS
    visited[v] = 1;

    // Fill questions if vertex is reachable
    if (net.questionToVertex[v].type == StringType::Pattern)
        net.pattern[net.questionToVertex[v].index] = '0';
    if (net.questionToVertex[v].type == StringType::Str)
        net.str[net.questionToVertex[v].index] = '0';

    // Launching search from other vertices
    for (int u = 0; u < net.numberOfVertices; u++)
        if (visited[u] == 0 &&
            net.matrix[v][u].capacity > net.matrix[v][u].flow)
            FindZeros(net, u, visited);
}



void buildNetwork(Network& net)
{
    // Builds network for finding Hamming distance
    // If 1 meets 0 -- Hamming distance is increased by 1
    // If ? meets 1 -- capacity in edge <vertex for this ?> - <0> is increased by 1
    // If ? meets 0 -- capacity in edge <vertex for this ?> - <0> is increased by 1
    // If ? meets ? -- capacity in edge <vertex for first ?> - <vertex for second ?> is increased by 1
    std::string &str = net.str;
    std::string &patt = net.pattern;
    int patternLength = (int) patt.length();
    for (int strPos = 0; strPos < str.length() - patt.length() + 1; strPos++) {
        for (int pattPos = 0; pattPos < patternLength; pattPos++) {

            // 1 meets 0
            if (patt[pattPos] == '1' && str[strPos + pattPos] == '0' ||
                patt[pattPos] == '0' && str[strPos + pattPos] == '1') {
                net.hammingDistance++;
            }

            // If there is no relation between this question and vertex, add this relation
            if (patt[pattPos] == '?') {
                if (net.vertexToQuestion[StringType::Pattern][pattPos] == -1) {
                    net.numberOfVertices++;
                    VertexInfo temp = {StringType::Pattern, pattPos};
                    net.questionToVertex[net.numberOfVertices - 1] = temp;
                    net.vertexToQuestion[StringType::Pattern][pattPos] = net.numberOfVertices - 1;
                }
            }
            if (str[strPos + pattPos] == '?') {
                if (net.vertexToQuestion[StringType::Str][strPos + pattPos] == -1) {
                    net.numberOfVertices++;
                    VertexInfo temp = {StringType::Str, strPos + pattPos};
                    net.questionToVertex[net.numberOfVertices - 1] = temp;
                    net.vertexToQuestion[StringType::Str][pattPos + strPos] = net.numberOfVertices - 1;
                }
            }

            // Check and process all possible combinations
            if (patt[pattPos] == '?') {
                switch (str[strPos + pattPos]) {
                    case '0':
                        net.increaseCapacity(0, net.vertexToQuestion[StringType::Pattern][pattPos]);
                        break;
                    case '1':
                        net.increaseCapacity(
                                net.vertexToQuestion[StringType::Pattern][pattPos], 1);
                        break;
                    case '?':
                        net.increaseCapacity(
                                net.vertexToQuestion[StringType::Pattern][pattPos],
                                net.vertexToQuestion[StringType::Str][strPos + pattPos]);
                        break;
                }
            }
            if (str[strPos + pattPos] == '?') {
                switch (patt[pattPos]) {
                    case '0':
                        net.increaseCapacity(0, net.vertexToQuestion[StringType::Str][strPos + pattPos]);
                        break;
                    case '1':
                        net.increaseCapacity(1, net.vertexToQuestion[StringType::Str][strPos + pattPos]);
                        break;
                }
            }
        }
    }
}