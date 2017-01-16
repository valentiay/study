#include <iostream>

using std::cin;
using std::cout;

int main(){
    int n;
    cin >> n;
    int * a = new int[n];
    int * b = new int[n];

    for(int i = 0; i < n; i++)
        cin >> a[i];
    for(int i = 0; i < n; i++)
        cin >> b[i];

    int aCurrMax = 0;
    int aMax = 0;
    int bMax = 0;
    for(int i = 1; i < n; i++){
        if(a[i] > a[aCurrMax])
            aCurrMax = i;
        if(a[aCurrMax] + b[i] > a[aMax] + b[bMax]){
            aMax = aCurrMax;
            bMax = i;
        }
    }

    cout << aMax << ' ' << bMax;

    delete[] a;
    delete[] b;
}
