#include <iostream>
#include <algorithm>

/******************************************************************************/

using std::vector;
using std::cin;
using std::cout;
using std::max;
using std::min;

/******************************************************************************/

class Tree{
public:

    Tree(int n);


    void    distance(vector <int> & out);

    void    insert(int x, int y);

private:

    struct Node{

        vector <Node*>  children_;

        int             i_;

        int             depth_;
    };

    int     maxChildrenDepth(Node * node, int except);


    void    fixDepth(Node * node);

    void    findDistance(Node * node, int maxDepth, vector <int> & out);


    vector <Node> nodes_;
};

/******************************************************************************/

int main(){
    int n;
    cin >> n;
    Tree tree(n);
    for(int i = 0; i < n - 1; i++){
        int x, y;
        cin >> x >> y;
        tree.insert(x, y);
    }
    vector <int> out(n);
    tree.distance(out);
    for(int i = 0; i < n; i++)
        cout << out[i] << '\n';
}

/****************************TREE**********************************************/

// Public:

Tree::Tree(int n):
    nodes_(n)
{
    for(int i = 0; i < n; i++)
        nodes_[i].i_ = i;
}



void Tree::distance(vector<int> & out){
    fixDepth(&nodes_[0]);
    findDistance(&nodes_[0], 0, out);
}



void Tree::insert(int x, int y){
    nodes_[min(x, y)].children_.push_back(&nodes_[max(x, y)]);
}

// Private:

int Tree::maxChildrenDepth(Node * node, int except){
    int maxDepth = 0;
    for(int i = 0; i < node->children_.size(); i++)
        if(node->children_[i]->i_ != except
           && node->children_[i]->depth_ > maxDepth){
            maxDepth = node->children_[i]->depth_;
        }
    return maxDepth;
}



void Tree::fixDepth(Node * node){
    if(node == nullptr)
        return;

    for(int i = 0; i < node->children_.size(); i++)
        fixDepth(node->children_[i]);

    node->depth_ = maxChildrenDepth(node, - 1) + 1;
}



void Tree::findDistance(Node * node, int maxDepth, vector<int> & out){
    if(node == nullptr)
        return;

    for(int i = 0; i < node->children_.size(); i++)
        findDistance(node->children_[i], max(maxChildrenDepth(node, node->children_[i]->i_), maxDepth) + 1, out);

    out[node->i_] = max(maxChildrenDepth(node, -1), maxDepth);
}
