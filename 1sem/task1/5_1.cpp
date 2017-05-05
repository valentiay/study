#include <iostream>

using std::string;
using std::cin;
using std::cout;

//Stack v0.3.2
template <typename T>
class Stack{
public:
    Stack();
    ~Stack();
    void                        push(T val);
    T                           pop();
    T                           min();
    T                           max();
    int                         size();
    T                           back();
    void                        clear();

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

//=============================================================================

bool matches(char L, char R);

bool isOpening(char val);

char getMatching(char val);

//=============================================================================

int main(){
    Stack <char> stack1, stack2;
    string str;

    cin >> str;
    int len = str.length();
    for(int i = 0; i < len; i++){
        if(str[i] == '\n')
            break;
        if(stack1.size() > 0 && matches(stack1.back(), str[i]))
            stack1.pop();
        else
            stack1.push(str[i]);
    }

    string beg;
    string end;

    while(stack1.size() > 0 && isOpening(stack1.back()))
        end.push_back(getMatching(stack1.pop()));

    while(stack1.size() > 0)
        popPush(stack1, stack2);

    while(stack2.size() > 0 && !isOpening(stack2.back()))
        beg.push_back(getMatching(stack2.pop()));

    if(stack2.size() == 0){
        len = beg.length();

        for (int i = 0; i < len; i++)
            cout << beg[len - 1 - i];
        cout << str;
        cout << end;
    }
    else{
        cout << "IMPOSSIBLE\n";
    }

    return 0;
}

//=============================================================================

bool matches(char R, char L){
    if(R == '(')
        return L == ')';
    else
        return R + 2 == L;
}



bool isOpening(char val){
    return val == '(' || val == '[' || val == '{';
}



char getMatching(char val){
    if(val == '(')
        return ')';
    if(val == ')')
        return '(';
    if(val == '{')
        return '}';
    if(val == '}')
        return '{';
    if(val == '[')
        return ']';
    return '[';
}

//===========================STACK=============================================

template <typename T>
Stack<T>::Stack(){
    top_ = NULL;
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
T Stack<T>::min(){
    if(size_ == 0)
        std::cerr << "Stack::min() : Empty stack\n";
    return top_->min;
}

template <typename T>
T Stack<T>::max(){
    if(size_ == 0)
        std::cerr << "Stack::max() : Empty stack\n";
    return top_->max;
}

template <typename T>
int Stack<T>::size(){
    return size_;
}

template <typename T>
T Stack<T>::back(){
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
    top_ = NULL;
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
        pushTo.top_->min = (pushTo.top_->next->min > pushTo.top_->val)?pushTo.top_->val:pushTo.top_->next->min;
        pushTo.top_->max = (pushTo.top_->next->max < pushTo.top_->val)?pushTo.top_->val:pushTo.top_->next->max;
    }
}
