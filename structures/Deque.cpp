#include <iostream>


// Deque v0.0.3
template <typename T>
class Deque{
public:
    Deque();

    ~Deque();


    void    push_front(T val);

    void    push_back(T val);

    T       pop_front();

    T       pop_back();

    void    clear();


    T       front()const;

    T       back() const;

    int     size() const;

private:
    struct Node{
        T       val;

        Node *  next;

        Node *  prev;
    };

    Node *  head_;

    Node *  tail_;

    int     size_;
};

/******************************************************************************/

/****************************DEQUE*********************************************/

template <typename T>
Deque<T>::Deque(){
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
}



template <typename T>
Deque<T>::~Deque(){
    clear();
}



template <typename T>
void Deque<T>::push_front(T val){
    Node * tmp = new Node;
    tmp -> val = val;
    tmp -> next = nullptr;
    if(!size_){
        tail_ = tmp;
        tmp -> prev = nullptr;
    }
    else{
        head_ -> next = tmp;
        tmp -> prev = head_;
    }
    head_ = tmp;
    size_++;
}



template <typename T>
void Deque<T>::push_back(T val){
    Node * tmp = new Node;
    tmp -> val = val;
    tmp -> prev = nullptr;
    if(!size_){
        head_ = tmp;
        tmp -> next = nullptr;
    }
    else{
        tail_ -> prev = tmp;
        tmp -> next = tail_;
    }
    tail_ = tmp;
    size_++;
}



template <typename T>
T Deque<T>::pop_front(){
    if(!size_){
        std::cerr << "Deque::pop_front : Empty deque\n";
    }
    Node * tmp = head_;
    if(size_ == 1)
        tail_ = nullptr;
    else
        head_ -> prev -> next = nullptr;
    head_ = tmp -> prev;
    size_--;
    T val = tmp -> val;
    delete tmp;
    return val;
}



template <typename T>
T Deque<T>::pop_back(){
    if(!size_){
        std::cerr << "Deque::pop_back : Empty deque\n";
    }
    Node * tmp = tail_;
    if(size_ == 1)
        head_ = nullptr;
    else
        tail_ -> next -> prev = nullptr;
    tail_ = tmp -> next;
    size_--;
    T val = tmp -> val;
    delete tmp;
    return val;
}



template <typename T>
T Deque<T>::front() const{
    if(!size_){
        std::cerr << "Deque::front : Empty deque\n";
    }
    return head_ -> val;
}



template <typename T>
T Deque<T>::back() const{
    if(!size_){
        std::cerr << "Deque::back : Empty deque\n";
    }
    return tail_ -> val;
}



template <typename T>
int Deque<T>::size() const{
    return size_;
}



template <typename T>
void Deque<T>::clear(){
    while(size_){
        Node * tmp = tail_;
        tail_ = tail_ -> next;
        size_--;
        delete tmp;
    }
}
