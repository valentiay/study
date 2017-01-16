#include <iostream>

using std::cout;
using std::cin;

//BST v0.1.2
template <typename T>
class BST{
public:
    BST();
    ~BST();

    void    insert(T val);
    void    clear();
    void    print();

    bool    deleteByKey(T key);
    bool    find(T key);

    T       findMin();
    T       findMax();

    int     height();

private:
    struct Node{
        Node *  left;
        Node *  right;
        T       val;
    };

    Node *  root_;

    void    clearRec(Node *node);
    void    printRec(Node * node, int depth);
    void    deleteNode(Node * & node);
    void    heightRec(Node * node, int height, int & max);

    Node *  findNode(T key);

};

/******************************************************************************/

// Treap v0.1.0
template <typename DataType, typename KeyType = DataType>
class Treap{
public:
    Treap();
    ~Treap();

    void        insert(int priority, KeyType key, DataType val);
    void        clear();
    void        print();

    bool        deleteByKey(KeyType key);

    DataType *  find(KeyType key);

    int     height();

    // insert function for situation, when KeyType == Datatype;

    void        insert(int priority, DataType key);

private:
    struct Node{
        KeyType     key;
        int         priority;
        DataType    val;
        Node *      left;
        Node *      right;
    };

    void    split(Node * node, KeyType key, Node * & left, Node * & right);
    void    clearRec(Node * node);
    void    printRec(Node * node, int depth);
    void    heightRec(Node * node, int height, int & max);

    Node *  merge(Node * left, Node * right);

    Node *  root_;

};

/****************************MAIN**********************************************/

int main(){
    int n;
    BST <int> bst;
    Treap <int> treap;
    cin >> n;
    for(int i = 0; i < n; i++){
        int x, y;
        cin >> x >> y;
        bst.insert(x);
        treap.insert(y, x);
    }
    cout << bst.height() - treap.height();
}

/****************************BST***********************************************/

// Public:

template <typename T>
BST<T>::BST():
        root_(NULL)
{}



template <typename T>
BST<T>::~BST(){
    clear();
}



template <typename T>
void BST<T>::insert(T val){
    Node * tmp = new Node;
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->val = val;

    if(root_ == NULL){
        root_ = tmp;
        return;
    }

    Node * node = root_;
    while(val < node->val && node->left != NULL
          || val >= node->val && node->right != NULL){
        node = (val < node->val)?(node->left):(node->right);
    }
    if(val < node->val)
        node->left = tmp;
    else
        node->right = tmp;
}



template <typename T>
void BST<T>::clear(){
    clearRec(root_);
    root_ = NULL;
}



template <typename T>
void BST<T>::print(){
    printRec(root_, 0);
}



template <typename T>
bool BST<T>::deleteByKey(T key){
    Node * node = root_;
    Node * parent = NULL;
    while(key != node->val
          && (key < node->val && node->left != NULL
              || key > node->val && node->right != NULL)){
        parent = node;
        node = (key <= node->val)?(node->left):(node->right);
    }
    if(node->val != key)
        return false;
    if(node != root_)
        deleteNode((key <= parent->val)?(parent->left):(parent->right));
    else
        deleteNode(root_);
    return true;
}



template <typename T>
bool BST<T>::find(T key){
    return (findNode(key) != NULL);
}



template <typename T>
T BST<T>::findMax(){
    Node * node = root_;
    while(node->right != NULL)
        node = node->right;
    return node->val;
}



template <typename T>
T BST<T>::findMin(){
    Node * node = root_;
    while(node->left != NULL)
        node = node->left;
    return node->val;
};



template <typename T>
int BST<T>::height(){
    int max = 0;
    heightRec(root_, 1, max);
    return max;
}

// Private:

template <typename T>
void BST<T>::printRec(Node * node, int depth){
    if(node == NULL)
        return;
    printRec(node->right, depth + 1);
    for(int i = 0; i < depth*3; i++)
        cout << ' ';
    cout << node->val << '\n';
    printRec(node->left, depth + 1);
}



template <typename T>
void BST<T>::clearRec(Node * node){
    if(node == NULL)
        return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}



template <typename T>
void BST<T>::deleteNode(Node * & node){
    if(node->right == NULL){
        Node * tmp = node->left;
        delete node;
        node = tmp;
    }
    else if(node->left == NULL){
        Node * tmp = node->right;
        delete node;
        node = tmp;
    }
    else{
        Node * minParent = node;
        Node * min = node->right;
        while(min->left != NULL){
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
    while(node != NULL && node->val != key){
        node = (key <= node->val)?(node->left):(node->right);
    }
    return node;
}



template <typename T>
void BST<T>::heightRec(Node * node, int height, int & max){
    if(node == NULL)
        return;
    if(height > max)
        max = height;
    heightRec(node->left, height + 1, max);
    heightRec(node->right, height + 1, max);
}

/****************************TREAP*********************************************/

// Public:

template <typename DataType, typename KeyType>
Treap<DataType, KeyType>::Treap():
        root_(NULL)
{}



template <typename DataType, typename KeyType>
Treap<DataType, KeyType>::~Treap(){
    clear();
}



template <typename DataType, typename KeyType>
void Treap<DataType, KeyType>::insert(int priority, KeyType key, DataType val){
    Node * left = NULL;
    Node * right = NULL;
    Node * tmp = new Node;

    Node * node = root_;
    Node * nodeFather = NULL;
    while (node != NULL && node->priority >= priority){
        nodeFather = node;
        node = (key < node->key)?(node->left):(node->right);
    }
    split(node, key, left, right);
    if(nodeFather == NULL)
        root_ = tmp;
    else if(key < nodeFather->key)
        nodeFather->left = tmp;
    else
        nodeFather->right = tmp;

    tmp->val = val;
    tmp->priority = priority;
    tmp->key = key;
    tmp->left = left;
    tmp->right = right;
}



template <typename DataType, typename KeyType>
void Treap<DataType, KeyType>::clear(){
    clearRec(root_);
}



template <typename DataType, typename KeyType>
void Treap<DataType, KeyType>::print(){
    printRec(root_, 0);
}



template <typename DataType, typename KeyType>
bool Treap<DataType, KeyType>::deleteByKey(KeyType key){
    Node * node = root_;
    Node * nodeFather = NULL;
    while (node != NULL && node->key != key){
        nodeFather = node;
        node = (key <= node->key)?(node->left):(node->right);
    }
    if(node == NULL)
        return false;
    Node * tmp = node;
    if(nodeFather == NULL)
        root_ = merge(node->left, node->right);
    else if(key <= nodeFather->key)
        nodeFather->left = merge(node->left, node->right);
    else
        nodeFather->right = merge(node->left, node->right);
    delete tmp;
    return true;
}



template <typename DataType, typename KeyType>
DataType * Treap<DataType, KeyType>::find(KeyType key){
    Node * node = root_;
    while (node != NULL && node->key != key)
        node = (key <= node->key)?(node->left):(node->right);
    if(node == NULL)
        return NULL;
    return &(node->val);
}



template <typename DataType, typename KeyType>
void Treap<DataType, KeyType>::insert(int priority, DataType key){
    insert(priority, key, key);
}



template <typename DataType, typename KeyType>
int Treap<DataType, KeyType>::height(){
    int max = 0;
    heightRec(root_, 1, max);
    return max;
}

// Private:

template <typename DataType, typename KeyType>
void Treap<DataType, KeyType>::clearRec(Node * node){
    if(node == NULL)
        return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}



template <typename DataType, typename KeyType>
void Treap<DataType, KeyType>::printRec(Node * node, int depth){
    if(node == NULL)
        return;
    printRec(node->right, depth + 1);
    for(int i = 0; i < depth*3; i++)
        cout << ' ';
    cout << node->val << "(" << node->key << "; "<< node->priority << ")\n";
    printRec(node->left, depth + 1);
}



template <typename DataType, typename KeyType>
void Treap<DataType, KeyType>::split(Node * node, KeyType key,
                                     Node * & left, Node * & right){
    if(node == NULL){
        left = NULL;
        right = NULL;
    }
    else if(node->key < key){
        split(node->right, key, node->right, right);
        left = node;
    }
    else{
        split(node->left, key, left, node->left);
        right = node;
    }
}



template <typename DataType, typename KeyType>
typename Treap<DataType, KeyType>::Node * Treap
        <DataType, KeyType>::merge(Node * left, Node * right){
    if(left == NULL || right == NULL)
        return (left == 0)?(right):(left);
    else if(left->priority > right->priority){
        left->right = merge(left->right, right);
        return left;
    }
    right->left = merge(left, right->left);
    return right;
}



template <typename DataType, typename KeyType>
void Treap<DataType, KeyType>::heightRec(Node * node, int height, int & max){
    if(node == NULL)
        return;
    if(height > max)
        max = height;
    heightRec(node->left, height + 1, max);
    heightRec(node->right, height + 1, max);
}
