#include <iostream>

using std::cin;
using std::cout;

template <typename T>
class Queue{
public:
            Queue();

            ~Queue();

    void    enqueue(T val);
    void    print();

    T       dequeue();

    int     size();

private:
    void    grow();

    T *     buffer_;

    int     bufferSize_;
    int     size_;
    int     beg_;
    int     end_;
};

/****************************MAIN**********************************************/

int main(){
    int n;
    bool success = true;
    cin >> n;
    Queue <int> queue;
    for(int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        if (a == 2) {
            if (b != ((queue.size() == 0) ? (-1) : (queue.dequeue())))
                success = false;
        }
        else
            queue.enqueue(b);
    }
    if(success)
        cout << "YES";
    else
        cout << "NO";
}

/****************************QUEUE*********************************************/

// Public:

template <typename T>
Queue<T>::Queue():
    bufferSize_(8),
    size_(0),
    beg_(0),
    end_(0)
{
    buffer_ = new T[bufferSize_];
}



template <typename T>
Queue<T>::~Queue(){
    delete[] buffer_;
}



template <typename T>
void Queue<T>::enqueue(T val){
    if(size_ != 0)
        end_ = (end_ + 1) % bufferSize_;
    buffer_[end_] = val;
    size_++;
    if(size_ == bufferSize_)
        grow();
}



template <typename T>
void Queue<T>::print(){
    for(int i = 0; i < bufferSize_; i++)
        std::cout << buffer_[i] << ' ';
    std::cout << '\n';
}



template <typename T>
T Queue<T>::dequeue(){
    T val = buffer_[beg_];
    if(size_ != 1)
        beg_ = (beg_ + 1) % bufferSize_;
    size_--;
    return val;
}



template <typename T>
int Queue<T>::size(){
    return size_;
}

// Private:

template <typename T>
void Queue<T>::grow(){
    T * tmp = buffer_;
    bufferSize_ *= 2;
    buffer_ = new T[bufferSize_];
    for(int i = beg_; i < size_; i++)
        buffer_[i - beg_] = tmp[i];
    if(beg_ != 0)
        for(int i = 0; i < end_ + 1; i++)
            buffer_[i + size_ - beg_] = tmp[i];
    beg_ = 0;
    end_ = size_ - 1;
    delete[] tmp;
}
