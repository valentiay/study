#include <iostream>

// Queue v0.0.3
template <typename T>
class Queue{
public:
    
    Queue();
    
    ~Queue();
    
    
    void    enqueue(T val);
    
    T       dequeue();

    void    clear();
    
    
    T       front() const;
    
    int     size() const;

private:
    
    struct Node{
        
        T       val;
        
        Node *  prev;
        
        Node *  next;
    };
    
    
    Node *  head_;
    
    Node *  tail_;
    
    int     size_;
};

/******************************************************************************/

/****************************QUEUE*********************************************/

template <typename T>
Queue<T>::Queue(){
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
}



template <typename T>
Queue<T>::~Queue(){
    clear();
}



template <typename T>
void Queue<T>::enqueue(T val){
    Node * tmp = new Node;
    tmp->val = val;
    tmp->prev = nullptr;
    tmp->next = nullptr;
    if(size_ == 0)
        head_ = tmp;
    else
        tail_->next = tmp;
    tmp->prev = tail_;
    tail_ = tmp;
    size_++;
}



template <typename T>
T Queue<T>::dequeue(){
    if(size_ == 0){
        std::cerr << "Queue::dequeue : empty queue \n";
    }
    Node * tmp = head_;
    if(size_ != 1){
        head_ = head_->next;
        head_->prev = nullptr;
    }
    else{
        head_ = nullptr;
        tail_ = nullptr;
    }
    T val = tmp->val;
    size_--;
    delete tmp;
    return val;
}



template <typename T>
T Queue<T>::front() const{
    if(size_ == 0){
        std::cerr << "Queue::front : empty queue \n";
    }
    return head_->val;
}



template <typename T>
int Queue<T>::size() const{
    return size_;
}



template <typename T>
void Queue<T>::clear(){
    while(size_ != 0){
        Node * tmp = tail_;
        tail_ = tail_->prev;
        delete tmp;
        size_--;
    }
    head_ = nullptr;
    tail_ = nullptr;
}
