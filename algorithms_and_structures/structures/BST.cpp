#include <iostream>

using std::cout;

//BST v1.0.0
template <typename T>
class BST{
public:
    BST();

    ~BST();


    void    insert(T key);

    bool    remove(T key);

    void    clear();


    bool    contains(T key) const;

    T       findMin() const;

    T       findMax() const;


    void    print() const;

private:
    struct Node{
        Node *  left;

        Node *  right;

        T      key;
    };


    void    clearRec(Node *node);

    void    deleteNode(Node * & node);

    Node *  findNode(T key);


    void    printRec(Node * node, int depth) const;


    Node *  root_;
};

/******************************************************************************/

/****************************BST***********************************************/

// Public:

template <typename T>
BST<T>::BST():
        root_(nullptr)
{}



template <typename T>
BST<T>::~BST(){
    clear();
}



template <typename T>
void BST<T>::insert(T key){
    Node * tmp = new Node;
    tmp->left = nullptr;
    tmp->right = nullptr;
    tmp->key = key;

    if(root_ == nullptr){
        root_ = tmp;
        return;
    }

    Node * node = root_;
    while(key <= node->key && node->left != nullptr
          || key > node->key && node->right != nullptr){
        node = (key <= node->key)?(node->left):(node->right);
    }
    if(key <= node->key)
        node->left = tmp;
    else
        node->right = tmp;
}



template <typename T>
void BST<T>::clear(){
    clearRec(root_);
    root_ = nullptr;
}



template <typename T>
void BST<T>::print() const{
    printRec(root_, 0);
}



template <typename T>
bool BST<T>::remove(T key){
    Node * node = root_;
    Node * parent = nullptr;
    while(key != node->key
          && (key < node->key && node->left != nullptr
              || key > node->key && node->right != nullptr)){
        parent = node;
        node = (key <= node->key)?(node->left):(node->right);
    }
    if(node->key != key)
        return false;
    if(node != root_)
        deleteNode((key <= parent->key)?(parent->left):(parent->right));
    else
        deleteNode(root_);
    return true;
}



template <typename T>
bool BST<T>::contains(T key) const{
    return (findNode(key) != nullptr);
}



template <typename T>
T BST<T>::findMax() const{
    Node * node = root_;
    while(node->right != nullptr)
        node = node->right;
    return node->key;
}



template <typename T>
T BST<T>::findMin() const{
    Node * node = root_;
    while(node->left != nullptr)
        node = node->left;
    return node->key;
};

// Private:

template <typename T>
void BST<T>::printRec(Node * node, int depth) const{
    if(node == nullptr)
        return;
    printRec(node->right, depth + 1);
    for(int i = 0; i < depth*3; i++)
        cout << ' ';
    cout << node->key << '\n';
    printRec(node->left, depth + 1);
}



template <typename T>
void BST<T>::clearRec(Node * node){
    if(node == nullptr)
        return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}



template <typename T>
void BST<T>::deleteNode(Node * & node){
    if(node->right == nullptr){
        Node * tmp = node->left;
        delete node;
        node = tmp;
    }
    else if(node->left == nullptr){
        Node * tmp = node->right;
        delete node;
        node = tmp;
    }
    else{
        Node * minParent = node;
        Node * min = node->right;
        while(min->left != nullptr){
            minParent = min;
            min = min->left;
        }
        if(min == node->right)
            minParent->right = min->right;
        else
            minParent->left = min->right;
        min->left = node->left;
        min->right = node->right;
        delete node;
        node = min;
    }
}



template <typename T>
typename BST<T>::Node * BST<T>::findNode(T key){
    Node * node = root_;
    while(node != nullptr && node->key != key){
        node = (key <= node->key)?(node->left):(node->right);
    }
    return node;
}
