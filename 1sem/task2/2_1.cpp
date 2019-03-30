#include <iostream>

using std::cin;
using std::cout;

template <class T>
struct Heap{
    Heap();

    ~Heap();

    void        siftUp(int index);
    void        siftDown(int index);
    void        insert(T val);
    void        clear();
    void        print();
    void        append(T val);
    void        sort();

    T           extract_max();
    T           get_max();

    int         size();

    T *         buffer;

    int         defaultSize;
    int         bufferSize;
    int         size_;
};

struct Customer{
    int beg;
    int end;
    int ads;

    Customer();
    Customer(int beg, int end);

    friend bool operator > (const Customer & L, const Customer & R);
};

//=============================================================================

void increase(Customer * arr, int n, int start, int adTime){
    for(int i = start; i < n; i++){
        if(arr[i].beg <= adTime)
            arr[i].ads++;
        else
            break;
    }
}

//=============================================================================

int main(){
    int n;
    cin >> n;
    Heap <Customer> heap;
    for(int i = 0; i < n; i++){
        int beg, end;
        cin >> beg >> end;
        Customer customer(beg, end);
        heap.append(customer);
    }
    heap.sort();
    Customer * arr = heap.buffer;
    int ads = 0;
    for(int i = 0; i < n; i++){
        if(arr[i].ads == 0){
            increase(arr, n, i, arr[i].end - 1);
            ads++;
        }
        if(arr[i].ads == 1){
            increase(arr, n, i, arr[i].end);
            ads++;
        }
    }

    cout << ads;
}

//===========================CUSTOMER==========================================

Customer::Customer(int beg, int end):
    beg(beg),
    end(end),
    ads(0)
{}

Customer::Customer(){}

bool operator > (const Customer & L, const Customer & R){
    if(L.end > R.end)
        return true;
    if(L.end == R.end)
        return L.beg < R.beg;
    return false;
}

//===========================HEAP==============================================

template <class T>
Heap<T>::Heap():
        defaultSize(16),
        bufferSize(defaultSize),
        size_(0)
{
    buffer = new T[bufferSize];
}

template <class T>
Heap<T>::~Heap(){
    delete[] buffer;
}

template <class T>
void Heap<T>::siftUp(int index){
    if(index >= this->size_)
        std::cerr << "Heap::siftUp : Wrong index\n";
    while(index && this->buffer[index] > this->buffer[(index - 1)/2]){
        T tmp = this->buffer[index];
        this->buffer[index] = this->buffer[(index - 1)/2];
        this->buffer[(index - 1)/2] = tmp;
        index = (index - 1)/2;
    }
}

template <class T>
void Heap<T>::siftDown(int index){
    if(index >= this->size_)
        std::cerr << "Heap::siftDown : Wrong index\n";
    int max = index;
    if(index*2 + 1 < this->size_ && this->buffer[index*2 + 1] > this->buffer[max])
        max = index*2 + 1;
    if(index*2 + 2 < this->size_ && this->buffer[index*2 + 2] > this->buffer[max])
        max = index*2 + 2;
    if(max != index){
        T tmp = this->buffer[index];
        this->buffer[index] = this->buffer[max];
        this->buffer[max] = tmp;
        siftDown(max);
    }
}

template <class T>
void Heap<T>::insert(T val){
    if(size_ + 1 > bufferSize){
        T * tmp = new T[bufferSize*2];
        for(int i = 0; i < bufferSize; i++)
            tmp[i] = buffer[i];
        delete[] buffer;
        buffer = tmp;
        bufferSize *= 2;
    }
    buffer[size_] = val;
    size_++;
    siftUp(size_ - 1);
}

template <class T>
T Heap<T>::extract_max(){
    if(!size_)
        std::cerr << "Heap::extract_top : Empty heap\n";
    T min = buffer[0];
    buffer[0] = buffer[size_ - 1];
    size_--;
    if(size_)
        siftDown(0);
    return min;
}

template <class T>
T Heap<T>::get_max(){
    if(!size_)
        std::cerr << "Heap::get_top : Empty heap\n";
    return buffer[0];
}

template <class T>
int Heap<T>::size(){
    return size_;
}

template <class T>
void Heap<T>::clear(){
    delete[] buffer;
    buffer = new T[defaultSize];
    bufferSize = defaultSize;
    size_ = 0;
}

template <class T>
void Heap<T>::print(){
    for(int i = 0; i < size_; i++)
        printf("%d ", buffer[i]);
    printf("\n");
}

template <class T>
void Heap<T>::append(T val){
    if(size_ + 1 > bufferSize){
        T * tmp = new T[bufferSize*2];
        for(int i = 0; i < bufferSize; i++)
            tmp[i] = buffer[i];
        delete[] buffer;
        buffer = tmp;
        bufferSize *= 2;
    }
    buffer[size_] = val;
    size_++;
    siftUp(size_ - 1);
}

template <class T>
void Heap<T>::sort(){
    int oldSize = size_;
    for(int i = size_ - 1; i >= 0; i--){
        T tmp = extract_max();
        buffer[i] = tmp;
    }
    size_ = oldSize;
}
