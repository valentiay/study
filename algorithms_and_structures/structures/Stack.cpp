#include <iostream>

// Stack v0.3.3
template <typename T>
class Stack{
public:

    Stack();

    ~Stack();


    void                        push(T val);

    T                           pop();

    void                        clear();


    T                           back() const;

    T                           min() const;

    T                           max() const;


    int                         size() const;


    template <typename Y>
    friend void popPush (Stack <Y> & popFrom, Stack <Y> & pushTo);

private:

    struct  Node{

        T       val;

        T       min;

        T       max;

        Node *  next;
    };

    Node *  top_;

    int     size_;

};

/******************************************************************************/

/****************************STACK*********************************************/

template <typename T>
Stack<T>::Stack(){
    top_ = nullptr;
    size_ = 0;
}



template <typename T>
Stack<T>::~Stack(){
    clear();
}



template <typename T>
void Stack<T>::push(T val){
    Node * tmp = new Node;
    tmp->val = val;
    tmp->next = top_;
    top_ = tmp;
    size_++;
    if(size_ <= 1){
        top_->min = val;
        top_->max = val;
    }
    else{
        top_->min = (top_->next->min > val)?val:top_->next->min;
        top_->max = (top_->next->max < val)?val:top_->next->max;
    }
}



template <typename T>
T Stack<T>::pop(){
    if(size_ == 0){
        std::cerr << "Stack::pop() : Empty stack\n";
    }
    Node * tmp = top_;
    top_ = top_->next;
    size_--;
    T val = tmp->val;
    delete tmp;
    return val;
}



template <typename T>
T Stack<T>::min() const{
    if(size_ == 0)
        std::cerr << "Stack::min() : Empty stack\n";
    return top_->min;
}



template <typename T>
T Stack<T>::max() const{
    if(size_ == 0)
        std::cerr << "Stack::max() : Empty stack\n";
    return top_->max;
}



template <typename T>
int Stack<T>::size() const{
    return size_;
}



template <typename T>
T Stack<T>::back() const{
    if(size_ == 0){
        std::cerr << "Stack::back() : Empty stack\n";
    }
    return top_->val;
}



template <typename T>
void Stack<T>::clear(){
    while(size_ != 0){
        Node * tmp = top_;
        top_ = top_->next;
        delete tmp;
        size_--;
    }
    top_ = nullptr;
}



template <typename T>
void popPush(Stack <T> & popFrom, Stack <T> & pushTo){
    if(popFrom.size_ == 0){
        std::cerr << "popPush() : Empty stack\n";
    }
    typename Stack<T>::Node * tmp = popFrom.top_;
    popFrom.top_ = popFrom.top_->next;
    popFrom.size_--;

    tmp->next = pushTo.top_;
    pushTo.top_ = tmp;
    pushTo.size_++;
    if(pushTo.size_ <= 1){
        pushTo.top_->min = pushTo.top_->val;
        pushTo.top_->max = pushTo.top_->val;
    }
    else{
        pushTo.top_->min = (pushTo.top_->next->min > pushTo.top_->val)
                           ?pushTo.top_->val:pushTo.top_->next->min;
        pushTo.top_->max = (pushTo.top_->next->max < pushTo.top_->val)
                           ?pushTo.top_->val:pushTo.top_->next->max;
    }
}
