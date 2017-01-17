#include <iostream>

using std::string;
using std::cin;
using std::cout;

const int k = 256;

void countingSort(string * a, int n, int * c, int byte);

void MSDSort(string * a, int n, int byte);

/******************************************************************************/

int main(){
    int n = 0;
    string * a = new string[100001];

    while(!cin.eof()){
        cin >> a[n++];
        if(a[n - 1].empty()){
            n--;
            break;
        }
    }

    MSDSort(a, n, 0);

    for(int i = 0; i < n; i++)
        cout << a[i] << '\n';

    delete[] a;
}

/******************************************************************************/

void countingSort(string * a, int n, int * c, int byte){
    for(int i = 0; i < k; i++)
        c[i] = 0;
    for(int i = 0; i < n; i++)
        c[a[i][byte]]++;

    int sum = 0;
    for(int i = 0; i < k; i++){
        int tmp = c[i];
        c[i] = sum;
        sum += tmp;
    }

    string * b = new string[n];
    for(int i = 0; i < n; i++){
        b[c[a[i][byte]]++] = a[i];
    }
    for(int i = 0; i < n; i++)
        a[i] = b[i];

    delete[] b;
}



void MSDSort(string * a, int n, int byte){
    if(n <= 1)
        return;

    int * c = new int[k + 1];
    c[k] = n - 1;

    countingSort(a, n, c, byte);

    for(int i = 1; i < k; i++)
        MSDSort(a + c[i], c[i + 1] - c[i], byte + 1);

    delete[] c;
}
