#include <iostream>

using namespace std;

int main(){
    int n, k;
    cin >> n >> k;
    k--;
    double * a = new double[k*(n - 1) + 1];
    for(int i = 0; i < n; i++)
        cin >> a[i*k];
    for(int i = 0; i < n - 1; i++)
        for(int j = 1; j < k; j++)
            a[i*k + j] = a[i*k] + j*(a[(i + 1)*k] - a[i*k]) / k;
    for(int i = 0; i < k*(n - 1) + 1; i++)
        cout << a[i] << ' ';
    delete[] a;
}
