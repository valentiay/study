#include <iostream>

// Heap v1.1.1
template <typename T>
class Heap{
public:

    Heap();

    ~Heap();


    void        insert(T val);

    void        clear();



    int         size() const;

    void        print() const;

protected:
    virtual void        siftUp(int index) = 0;

    virtual void        siftDown(int index) = 0;


    T           extract_top();

    T           get_top() const;


    T *         buffer_;

    int         defaultSize_;

    int         bufferSize_;

    int         size_;
};

/******************************************************************************/

/****************************HEAP**********************************************/

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
T Heap<T>::get_top() const{
    if(!size_)
        std::cerr << "Heap::get_top : Empty heap\n";
    return buffer_[0];
}



template <typename T>
int Heap<T>::size() const{
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
void Heap<T>::print() const{
    for(int i = 0; i < size_; i++)
        printf("%d ", buffer_[i]);
    printf("\n");
}
