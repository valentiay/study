#include <vector>
#include <iostream>

using std::vector;
using std::cin;
using std::cout;

// Returns stok vertex or n, if there is no stok
int findStok(vector<vector<int>> graph)
{
    unsigned int n = graph.size();

    // Stores true if vertex can be stok, false otherwise
    vector<bool> stok(n, true);

    for(int v = 0; v < n; v++){
        // Skipping vertex if it can't be stok
        if(!stok[v])
            continue;

        if(graph[v][v] == 0){
            // Checking line in table
            int i = (v + 1) % n;
            while(i != v){
                if(graph[v][i] == 0){
                    // Vertex k cant't be stok because its column has 0
                    stok[i] = false;
                }
                else{
                    // Vertex from can't be stok because its line has 1
                    stok[v] = false;
                    break;
                }
                i = (i + 1) % n;
            }

            if(i == v){
                // If line has no 1, checking column
                for(int m = 0; m < n; m++)
                    if(graph[m][v] == 0 && m != v){
                        // If column has 0 vertex isn't stok, other
                        // vertices also can't be stok
                        return n;
                    }
                // If line has no 1 and column has no 0, vertex is stok
                return v;
            }
        }
        else{
            // If vertex has outgoing edge, it is not stok
            stok[v] = false;
        }
    }

    // If stok is not found, there is no stock
    return n;
}

int main()
{
    unsigned int n = 0;
    cin >> n;

    // Reading table
    vector<vector<int>> graph(n, vector<int>(n, 0));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            cin >> graph[i][j];

    if(findStok(graph) != n)
        cout << "YES";
    else
        cout << "NO";
    return 0;
}