#include <iostream>
#include "../include/CListGraph.h"
#include "../include/CMatrixGraph.h"
#include "../include/CSetGraph.h"
#include "../include/CArcGraph.h"

/******************************************************************************/

using std::cin;
using std::cout;

/******************************************************************************/

int main(){
    unsigned int n;
    cin >> n;
    CListGraph graph(n);
    bool finish = false;
    while(!finish){
        char c;
        cin >> c;
        if(c == '+'){
            int a, b;
            cin >> a >> b;
            graph.AddEdge(a, b);
        }
        else
            finish = true;
    }

    finish = false;
    CListGraph listGraph(&graph);
    CMatrixGraph matrixGraph(&graph);
    CSetGraph setGraph(&graph);
    CArcGraph arcGraph(&graph);
    while(!finish){
        char c;
        int a, b;
        cin >> c;
        vector<int> vertices1;
        vector<int> vertices2;
        vector<int> vertices3;
        vector<int> vertices4;
        switch(c){
            case '+':
                cin >> a >> b;
                listGraph.AddEdge(a, b);
                matrixGraph.AddEdge(a, b);
                setGraph.AddEdge(a, b);
                arcGraph.AddEdge(a, b);
                break;
            case '?':
                cout << "List: " << listGraph.VerticesCount()
                     << "\nMatrix: " << matrixGraph.VerticesCount()
                     << "\nSet: " << setGraph.VerticesCount()
                     << "\nArc: " << arcGraph.VerticesCount()
                     << '\n';
                break;
            case '>':
                cin >> a;
                listGraph.GetNextVertices(a, vertices1);
                matrixGraph.GetNextVertices(a, vertices2);
                setGraph.GetNextVertices(a, vertices3);
                arcGraph.GetNextVertices(a, vertices4);
                cout << "List: ";
                for(int i : vertices1)
                    cout << i << ' ';
                cout << "\nMatrix: ";
                for(int i : vertices2)
                    cout << i << ' ';
                cout << "\nSet: ";
                for(int i : vertices3)
                    cout << i << ' ';
                cout << "\nArc: ";
                for(int i : vertices4)
                    cout << i << ' ';
                cout << '\n';
                break;
            case '<':
                cin >> a;
                listGraph.GetPrevVertices(a, vertices1);
                matrixGraph.GetPrevVertices(a, vertices2);
                setGraph.GetPrevVertices(a, vertices3);
                arcGraph.GetPrevVertices(a, vertices4);
                cout << "List: ";
                for(int i : vertices1)
                    cout << i << ' ';
                cout << "\nMatrix: ";
                for(int i : vertices2)
                    cout << i << ' ';
                cout << "\nSet: ";
                for(int i : vertices3)
                    cout << i << ' ';
                cout << "\nArc: ";
                for(int i : vertices4)
                    cout << i << ' ';
                cout << '\n';
                break;
            default:
                finish = true;
        }
    }
    return 0;
}