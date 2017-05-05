#include <iostream>

using std::cin;
using std::cout;

int find(int * const arr, int key, int p, int r){
    int c = (p + r)/2;
    if(arr[c] < key)
        return find(arr, key, c + 1, p);
    else if(key <= arr[c] && (c - 1 < 0 || arr[c - 1] < key))
        return c;
    else
        return find(arr, key, p, c - 1);
}

int main(){
    int n, m;
    cin >> n >> m;
    int * a = new int[n];
    int * b = new int[m];

    for(int i = 0; i < n; i++)
        cin >> a[i];
    for(int i = 0; i < m; i++)
        cin >> b[i];

    for(int i = 0; i < m; i++)
        if(b[i] <= a[n - 1])
            cout << find(a, b[i], 0, n - 1) << ' ';
        else
            cout << n << ' ';

    delete[] a;
    delete[] b;
}
