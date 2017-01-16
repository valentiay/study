#include <iostream>

using std::cin;
using std::cout;

//Heap v1.0.1
template <typename T>
class Heap{
public:
    Heap();
    ~Heap();
    void        insert(T val);
    int         size();
    void        clear();
    void        print();
    void        append(T val);

protected:
    virtual void        siftUp(int index) = 0;
    virtual void        siftDown(int index) = 0;

    T           extract_top();
    T           get_top();

    T *         buffer_;
    int         defaultSize_;
    int         bufferSize_;
    int         size_;
};

//=============================================================================

//Heap v1 Required

//MaxHeap v0.0.1
template <typename T>
class MaxHeap: public Heap<T>{
private:
    void        siftUp(int index);
    void        siftDown(int index);

public:
    T           extract_max();
    T           get_max();
};

//=============================================================================

int main(){
    long n, k;
    cin >> n >> k;
    MaxHeap <int> heap;

    for(long i = 0; i < k; i++){
        int x;
        cin >> x;
        heap.append(x);
    }
    for(long i = k; i < n; i++){
        int x;
        cin >> x;
        if(heap.get_max() > x){
            heap.extract_max();
            heap.append(x);
        }
    }

    int * result = new int[k];
    for(long i = k - 1; i >= 0; i--)
        result[i] = heap.extract_max();

    for(int i = 0; i < k; i++)
        cout << result[i] << ' ';

    delete[] result;
    return 0;
}

//===========================MAXHEAP===========================================

template <typename T>
void MaxHeap<T>::siftUp(int index){
    if(index >= this->size_)
        std::cerr << "Heap::siftUp : Wrong index\n";
    while(index && this->buffer_[index] > this->buffer_[(index - 1)/2]){
        T tmp = this->buffer_[index];
        this->buffer_[index] = this->buffer_[(index - 1)/2];
        this->buffer_[(index - 1)/2] = tmp;
        index = (index - 1)/2;
    }
}

template <typename T>
void MaxHeap<T>::siftDown(int index){
    if(index >= this->size_)
        std::cerr << "Heap::siftDown : Wrong index\n";
    int max = index;
    if(index*2 + 1 < this->size_ && this->buffer_[index*2 + 1] > this->buffer_[max])
        max = index*2 + 1;
    if(index*2 + 2 < this->size_ && this->buffer_[index*2 + 2] > this->buffer_[max])
        max = index*2 + 2;
    if(max != index){
        T tmp = this->buffer_[index];
        this->buffer_[index] = this->buffer_[max];
        this->buffer_[max] = tmp;
        siftDown(max);
    }
}

template <typename T>
T MaxHeap<T>::extract_max(){
    return this->extract_top();
}

template <typename T>
T MaxHeap<T>::get_max(){
    return this->get_top();
}

//===========================HEAP==============================================

template <typename T>
Heap<T>::Heap():
        defaultSize_(16),
        bufferSize_(defaultSize_),
        size_(0)
{
    buffer_ = new T[bufferSize_];
}

template <typename T>
Heap<T>::~Heap(){
    delete[] buffer_;
}

template <typename T>
void Heap<T>::insert(T val){
    if(size_ + 1 > bufferSize_){
        T * tmp = new T[bufferSize_*2];
        for(int i = 0; i < bufferSize_; i++)
            tmp[i] = buffer_[i];
        delete[] buffer_;
        buffer_ = tmp;
        bufferSize_ *= 2;
    }
    buffer_[size_] = val;
    size_++;
    siftUp(size_ - 1);
}

template <typename T>
T Heap<T>::extract_top(){
    if(!size_)
        std::cerr << "Heap::extract_top : Empty heap\n";
    T min = buffer_[0];
    buffer_[0] = buffer_[size_ - 1];
    size_--;
    if(size_)
        siftDown(0);
    return min;
}

template <typename T>
T Heap<T>::get_top(){
    if(!size_)
        std::cerr << "Heap::get_top : Empty heap\n";
    return buffer_[0];
}

template <typename T>
int Heap<T>::size(){
    return size_;
}

template <typename T>
void Heap<T>::clear(){
    delete[] buffer_;
    buffer_ = new T[defaultSize_];
    bufferSize_ = defaultSize_;
    size_ = 0;
}

template <typename T>
void Heap<T>::print(){
    for(int i = 0; i < size_; i++)
        printf("%d ", buffer_[i]);
    printf("\n");
}

template <typename T>
void Heap<T>::append(T val){
    if(size_ + 1 > bufferSize_){
        T * tmp = new T[bufferSize_*2];
        for(int i = 0; i < bufferSize_; i++)
            tmp[i] = buffer_[i];
        delete[] buffer_;
        buffer_ = tmp;
        bufferSize_ *= 2;
    }
    size_++;
    buffer_[size_ - 1] = val;
    siftUp(size_ - 1);
}
