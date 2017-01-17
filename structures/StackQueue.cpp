// Stack v0.3 required

// StackQueue v0.4.0
template <typename T>
class StackQueue{
public:

    StackQueue();

    ~StackQueue();


    void    enqueue(T val);

    T       dequeue();

    void    clear();


    T       front() const;

    T       min() const;

    T       max() const;


    int     size() const;


private:

    void        shift();


    Stack <T>   stack1_;

    Stack <T>   stack2_;

    int         size_;
};

/******************************************************************************/

/****************************STACKQUEUE****************************************/

template <typename T>
StackQueue<T>::StackQueue(){
    size_ = 0;
}



template <typename T>
StackQueue<T>::~StackQueue(){
    clear();
}



template <typename T>
void StackQueue<T>::enqueue(T val){
    stack1_.push(val);
    size_++;
}



template <typename T>
void StackQueue<T>::shift(){
    while(stack1_.size()){
        popPush(stack1_, stack2_);
    }
}



template <typename T>
T StackQueue<T>::dequeue(){
    if(size_ == 0){
        std::cerr << "StackQueue::dequeue : empty queue \n";
    }
    if(!stack2_.size())
        shift();
    size_--;
    return stack2_.pop();
}



template <typename T>
T StackQueue<T>::front() const{
    if(size_ == 0){
        std::cerr << "StackQueue::front : empty queue \n";
    }
    if(!stack2_.size())
        shift();
    return stack2_.back();
}



template <typename T>
int StackQueue<T>::size() const{
    return size_;
}



template <typename T>
void StackQueue<T>::clear(){
    stack1_.clear();
    stack2_.clear();
    size_ = 0;
}



template <typename T>
T StackQueue<T>::min()const{
    if(size_ == 0){
        std::cerr << "Queue::min : empty queue \n";
    }
    if(stack1_.size() == 0)
        return stack2_.min();
    else if(stack2_.size() == 0)
        return stack1_.min();
    else
        return std::min(stack1_.min(), stack2_.min());
}



template <typename T>
T StackQueue<T>::max()const{
    if(size_ == 0){
        std::cerr << "Queue::max : empty queue \n";
    }
    if(stack1_.size() == 0)
        return stack2_.max();
    else if(stack2_.size() == 0)
        return stack1_.max();
    else
        return std::max(stack1_.max(), stack2_.max());
}
