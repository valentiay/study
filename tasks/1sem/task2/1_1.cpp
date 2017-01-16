#include <iostream>

using namespace std;

bool fit(double* first, double* second){
    if(first[1] < second[1] && first[2] < second[2] && first[3] < second[3] ||
            first[1] < second[2] && first[2] < second[3] && first[3] < second[1] ||
            first[1] < second[3] && first[2] < second[1] && first[3] < second[2] ||
            first[1] < second[1] && first[2] < second[3] && first[3] < second[2] ||
            first[1] < second[2] && first[2] < second[1] && first[3] < second[3] ||
            first[1] < second[3] && first[2] < second[2] && first[3] < second[1]){
        return true;
    }
    return false;
}

int main() {
    int n;
    cin >> n;
    double** a = new double*[n];
    for(int i = 0; i < n; i++){
        a[i] = new double[4];
    }

    for(int i = 0; i < n; i++){
        a[i][0] = i;
        cin >> a[i][1] >> a[i][2] >> a[i][3];
    }

    for(int i = 1; i < n; i++){
        double* x = a[i];
        int j = i -1;
        while(j >= 0 && fit(x, a[j])){
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = x;
    }

    for(int i = 0; i < n; i++){
        cout << a[i][0] << ' ';
    }

    for(int i = 0; i < n; i++){
        delete[] a[i];
    }
    delete[] a;
}
