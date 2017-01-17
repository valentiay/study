// Heap v1 Required

// MinHeap v0.0.2
template <typename T>
class MinHeap: public Heap<T>{
private:
    
    void        siftUp(int index);
    
    void        siftDown(int index);

public:
    
    T           extract_min();
    
    T           get_min() const;
};

/******************************************************************************/

/****************************MINHEAP*******************************************/

template <typename T>
void MinHeap<T>::siftUp(int index){
    if(index >= this->size_)
        std::cerr << "Heap::siftUp : Wrong index\n";
    while(index &&  this->buffer_[index] < this->buffer_[(index - 1)/2]){
        T tmp = this->buffer_[index];
        this->buffer_[index] = this->buffer_[(index - 1)/2];
        this->buffer_[(index - 1)/2] = tmp;
        index = (index - 1)/2;
    }
}



template <typename T>
void MinHeap<T>::siftDown(int index){
    if(index >= this->size_)
        std::cerr << "Heap::siftDown : Wrong index\n";
    int min = index;
    if(index*2 + 1 < this->size_ 
       && this->buffer_[index*2 + 1] < this->buffer_[min])
        min = index*2 + 1;
    if(index*2 + 2 < this->size_ 
       && this->buffer_[index*2 + 2] < this->buffer_[min])
        min = index*2 + 2;
    if(min != index){
        T tmp = this->buffer_[index];
        this->buffer_[index] = this->buffer_[min];
        this->buffer_[min] = tmp;
        siftDown(min);
    }
}



template <typename T>
T MinHeap<T>::extract_min(){
    return this->extract_top();
}



template <typename T>
T MinHeap<T>::get_min() const{
    return this->get_top();
}
