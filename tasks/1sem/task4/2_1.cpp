#include <iostream>

using std::cin;
using std::cout;
using std::swap;
using std::min;

template <class T>
int partition(T * array, int p, int r);

template <typename T>
void quickSort(T * array, int p, int r);

struct Man{
    int weight;
    int strength;

    friend bool operator < (const Man & L, const Man & R);
    friend bool operator > (const Man & L, const Man & R);
};

/****************************MAIN**********************************************/

int main(){
    Man * men = new Man[100000];
    int n = 0;
    while(!cin.eof()){
        cin >> men[n].weight;
        cin >> men[n].strength;
        n++;
//        cout << "W:" << men[n - 1].weight << '\n';
//        if(cin.eof())
//            cout << "EOF\n";
    }

    n--;
    //cout << n << '\n';

    quickSort(men, 0, n - 1);

    int maxWeight = men[0].strength;
    int k = 1;
    for(int i = 1; i < n; i++){
        if(men[i].weight <= maxWeight){
            maxWeight = min(maxWeight - men[i].weight, men[i].strength);
            k++;
        }
    }

    cout << k << '\n';

//    for(int i = 0; i < n; i++)
//        cout << men[i].weight << ' ' << men[i].strength << '\n';

    return 0;
}

/****************************MAN***********************************************/

bool operator < (const Man & L, const Man & R){
    if(L.strength < R.strength)
        return true;
    else if(L.strength == R.strength)
        return L.weight > R.weight;
    return false;
}



bool operator > (const Man & L, const Man & R){
    if(L.strength > R.strength)
        return true;
    else if(L.strength == R.strength)
        return L.weight < R.weight;
    return false;
}

/****************************QSORT*********************************************/

template <class T>
int partition(T * array, int p, int r){
    int pivot;
    if(array[p] > array[(r + p)/ 2] && array[(r + p)/ 2] < array[r])
        pivot = (r + p) / 2;
    else if(array[r] < array[p] && array[p] < array[(r + p)/ 2])
        pivot = p;
    else
        pivot = r;

    T tmp = array[pivot];
    array[pivot] = array[r];
    array[r] = tmp;

    int j = p;
    for(int i = p; i < r; i++)
        if(array[i] > array[r]){
            tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
            j++;
        }

    tmp = array[j];
    array[j] = array[r];
    array[r] = tmp;
    return j;
}

template <typename T>
void quickSort(T * array, int p, int r){
    if(r <= p)
        return;
    int q = partition(array, p, r);
    quickSort(array, p, q - 1);
    quickSort(array, q + 1, r);
}
