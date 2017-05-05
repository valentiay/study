#include <vector>
#include <fstream>

/******************************************************************************/

using std::vector;
using std::min;

/******************************************************************************/

// Floyd algorithm
void floyd(vector<vector<double>> & graph){
    for(int k = 0; k < graph.size(); k++)
        for(int i = 0; i < graph.size(); i++)
            for(int j = 0; j < graph.size(); j++)
                graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
}

/******************************************************************************/

int main() {
    std::ifstream fin("floyd.in");
    std::ofstream fout("floyd.out");

    // Initializing graph
    unsigned n = 1;
    fin >> n;
    vector<vector<double>> graph(n, vector<double>(n, 0));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++){
            double a = 101;
            fin >> a;
            graph[i][j] = a;
        }

    // Starting floyd algorithm
    floyd(graph);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++)
            fout << graph[i][j] << ' ';
        fout << '\n';
    }

    fin.close();
    fout.close();

    return 0;
}