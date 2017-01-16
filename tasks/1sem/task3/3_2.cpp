#include <iostream>

using std::cout;
using std::cin;

// AVL v0.1.0
template <typename DataType, typename KeyType = DataType>
class AVL{
public:
    AVL();
    ~AVL();

    void        insert(KeyType key, DataType val);
    void        clear();
    void        print();

    bool        deleteByKey(KeyType key);

    DataType    kStat(int k);

    DataType *  find(KeyType key);

    // insert function for situation, when KeyType == DataType;

    void        insert(DataType key);

private:
    struct Node{
        Node(KeyType key, DataType val, Node * father);

        int         bfactor();
        void        fix();

        KeyType     key;

        int         height;
        int         leftSize;
        int         rightSize;

        DataType    val;

        Node *      father;
        Node *      left;
        Node *      right;
    };

    void    clearRec(Node * node);
    void    printRec(Node * node, int depth);
    void    balance(Node * node);
    void    rotateRight(Node * node);
    void    rotateLeft(Node * node);
    void    deleteNode(Node * & node);

    Node *  root_;
};

/****************************MAIN**********************************************/

int main(){
    AVL <int> avl;
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        int x, k;
        cin >> x >> k;
        if(x < 0)
            avl.deleteByKey(-x);
        else
            avl.insert(x);
        cout << avl.kStat(k) << ' ';
    }
}

/****************************AVL***********************************************/

// Public:

template <typename DataType, typename KeyType>
AVL<DataType, KeyType>::AVL():
        root_(NULL)
{}



template <typename DataType, typename KeyType>
AVL<DataType, KeyType>::~AVL(){
    clear();
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::insert(KeyType key, DataType val){
    Node * node = root_;
    Node * nodeFather = NULL;

    while(node != NULL){
        nodeFather = node;
        node = (node->key > key)?(node->left):(node->right);
    }

    Node * tmp = new Node(key, val, nodeFather);
    if(nodeFather == NULL)
        root_ = tmp;
    else if(nodeFather->key > key)
        nodeFather->left = tmp;
    else
        nodeFather->right = tmp;

    node = nodeFather;
    while(node != NULL){
        balance(node);
        node = node->father;
    }
    balance(root_);
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::clear(){
    clearRec(root_);
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::print(){
    printRec(root_, 0);
}



template <typename DataType, typename KeyType>
bool AVL<DataType, KeyType>::deleteByKey(KeyType key){
    Node * node = root_;
    Node * nodeFather = NULL;

    while(node->key != key && node != NULL){
        nodeFather = node;
        node = (key < node->key)?(node->left):(node->right);
    }

    if(node == NULL)
        return false;

    if(nodeFather == NULL)
        deleteNode(root_);
    else if(key < nodeFather->key)
        deleteNode(nodeFather->left);
    else
        deleteNode(nodeFather->right);

    node = nodeFather;
    while(node != NULL){
        balance(node);
        node = node->father;
    }
    balance(root_);
    return true;
}



template <typename DataType, typename KeyType>
DataType AVL<DataType, KeyType>::kStat(int k){
    Node * node = root_;
    while(k != node->leftSize){
        if(node->leftSize < k){
            k -= node->leftSize + 1;
            node = node->right;
        }
        else
            node = node->left;
    }
    return node->val;
}



template <typename DataType, typename KeyType>
DataType * AVL<DataType, KeyType>::find(KeyType key){
    Node * node = root_;
    while (node != NULL && node->key != key)
        node = (key < node->key)?(node->left):(node->right);
    if(node == NULL)
        return NULL;
    return &(node->val);
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::insert(DataType key){
    insert(key, key);
}

// Private:

template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::clearRec(Node * node){
    if(node == NULL)
        return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::printRec(Node * node, int depth){
    if(node == NULL)
        return;
    printRec(node->right, depth + 1);
    for(int i = 0; i < depth*3; i++)
        cout << ' ';
    cout << node->val << "  F: " << node->father << "  T: " << node
         <<"  L: " << node->leftSize << "  R: "
         << node->rightSize << "  H: " << node->height << "\n";
    printRec(node->left, depth + 1);
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::balance(Node * node){
    if(node == NULL)
        return;
    node->fix();
    if(node->bfactor() == 2){
        if(node->right->bfactor() == -1)
            rotateRight(node->right);
        rotateLeft(node);
    }
    else if(node->bfactor() == -2){
        if(node->left->bfactor() == 1)
            rotateLeft(node->left);
        rotateRight(node);
    }
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::rotateRight(Node * node){
    Node * C = node->left->right;
    Node * a = node;
    Node * b = node->left;

    b->father = a->father;
    if(b->father == NULL)
        root_ = b;
    else if(b->father->left == a)
        b->father->left = b;
    else
        b->father->right = b;

    a->father = b;
    b->right = a;

    if(C != NULL){
        C->father = a;
        a->left = C;
    }
    else
        a->left = NULL;

    a->fix();
    b->fix();
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::rotateLeft(Node * node){
    Node * C = node->right->left;
    Node * b = node;
    Node * a = node->right;

    a->father = b->father;
    if(a->father == NULL)
        root_ = a;
    else if(a->father->left == b)
        a->father->left = a;
    else
        a->father->right = a;

    b->father = a;
    a->left = b;

    if(C != NULL){
        C->father = b;
        b->right = C;
    }
    else
        b->right = NULL;

    b->fix();
    a->fix();
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::deleteNode(Node * & node){
    Node * nodeFather = node->father;
    Node * left = node->left;
    Node * right = node->right;
    delete node;

    if(left == NULL){
        node = right;
        if(right != NULL) right->father = nodeFather;
    }
    else if(right == NULL){
        node = left;
        left->father = nodeFather;
    }
    else{
        Node * minNode = right;
        while (minNode->left != NULL)
            minNode = minNode->left;

        if(right == minNode){
            node = minNode;
            node->father = nodeFather;
            node->left = left;
            left->father = node;
            balance(node);
        }
        else{
            node = minNode;
            node->father->left = node->right;
            if(node->right != NULL)
                node->right->father = node->father;

            minNode = node->father;
            node->father = nodeFather;
            node->left = left;
            left->father = node;
            node->right = right;
            right->father = node;

            while(minNode != nodeFather){
                balance(minNode);
                minNode = minNode->father;
            }
        }
    }
}

/****************************AVL::NODE*****************************************/

template <typename DataType, typename KeyType>
AVL<DataType, KeyType>::Node::Node(KeyType key, DataType val, Node * father):
        key(key),
        val(val),
        father(father),
        left(NULL),
        right(NULL),
        height(1),
        rightSize(0),
        leftSize(0)
{}



template <typename DataType, typename KeyType>
int AVL<DataType, KeyType>::Node::bfactor(){
    return ((right == NULL)?(0):(right->height)) -
           ((left == NULL)?(0):(left->height));
}



template <typename DataType, typename KeyType>
void AVL<DataType, KeyType>::Node::fix(){
    int hl = (left == NULL)?(0):(left->height);
    int hr = (right == NULL)?(0):(right->height);
    height = ((hl > hr)?(hl):(hr)) + 1;
    leftSize = (left == NULL)?(0):(left->leftSize + left->rightSize + 1);
    rightSize = (right == NULL)?(0):(right->leftSize + right->rightSize + 1);
}
