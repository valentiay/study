#include <iostream>
#include <iomanip>

// Treap with implicit key
class ImplicitTreap{
public:
    ImplicitTreap();
    ~ImplicitTreap();

    // Inserts string into position
    void insert(size_t position, const std::string& value);
    // Removes string by position
    void remove(size_t position);
    // Clears tree
    void clear();
    // Returns string by position
    std::string get(size_t position) const;

private:
    // Structure implementing node
    struct Node{
        Node(const std::string& value);

        // Fixes treeSize
        void fix();

        // Stores size of subtree
        size_t treeSize;
        // Priority of current node
        int priority;
        // Value to store
        std::string value;

        // Pointers tp left and right children
        Node* left;
        Node* right;
    };

private:
    // Splits tree by position, returns two result trees
    std::pair<Node*, Node*> split(Node* node, size_t position);
    // Merges two trees, returns result tree
    Node* merge(Node* node1, Node* node2);
    // Recursive function for clearing the tree
    void clear(Node* node);

private:
    // Pointer to root
    Node* _root;
};

/******************************************************************************/

int main() {
    size_t n = 0;
    std::cin >> n;
    ImplicitTreap stringArray;
    for (size_t i = 0; i < n; i++) {
        char cmd;
        std::cin >> cmd;
        switch(cmd){
            case '+': {
                size_t position = 0;
                std::string val = "";
                std::cin >> position >> val;
                stringArray.insert(position, val);
                break;
            }
            case '?': {
                size_t position = 0;
                std::cin >> position;
                std::cout << stringArray.get(position) << '\n';
                break;
            }
            default: {
                size_t position = 0;
                std::cin >> position;
                stringArray.remove(position);
                break;
            }
        }
    }
    return 0;
}

/******************************************************************************/

ImplicitTreap::ImplicitTreap()
        : _root(nullptr)
{}



ImplicitTreap::~ImplicitTreap()
{
    clear();
}



void ImplicitTreap::clear()
{
    clear(_root);
    _root = nullptr;
}



void ImplicitTreap::clear(Node* node)
{
    if (node == nullptr)
        return;
    clear(node->left);
    clear(node->right);
    delete node;
}



void ImplicitTreap::insert(size_t position, const std::string& value)
{
    Node* current = new Node(value);
    // Split tree by position and merge new node with right tree then merge result with left tree
    std::pair<Node*, Node*> ans = split(_root, position);
    Node* right = merge(current, ans.second);
    _root = merge(ans.first, right);
}



void ImplicitTreap::remove(size_t position)
{
    // Split required node from both sides and merge two other trees
    std::pair<Node*, Node*> ans1 = split(_root, position);
    std::pair<Node*, Node*> ans2 = split(ans1.second, 1);
    delete ans2.first;
    _root = merge(ans1.first, ans2.second);
}



std::string ImplicitTreap::get(size_t position) const
{
    // Going down the tree
    size_t l;
    Node* node = _root;
    do {
        l = (node->left != nullptr)?(node->left->treeSize):0;
        if (l == position)
            break;
        if (position < l) {
            node = node->left;
        }
        else {
            position -= l + 1;
            node = node->right;
        }
    } while (true);
    return node->value;
}



std::pair<ImplicitTreap::Node*, ImplicitTreap::Node*> ImplicitTreap::split(Node* node, size_t position)
{
    if (node == nullptr)
        return std::pair<Node*, Node*>(nullptr, nullptr);
    size_t l = (node->left != nullptr)?(node->left->treeSize) : 0;
    if (l >= position) {
        std::pair<Node*, Node*> ans = split(node->left, position);
        node->left = ans.second;
        node->fix();
        return std::make_pair(ans.first, node);
    }
    else {
        std::pair<Node*, Node*> ans = split(node->right, position - l - 1);
        node->right = ans.first;
        node->fix();
        return std::make_pair(node, ans.second);
    }
}



ImplicitTreap::Node* ImplicitTreap::merge(Node* node1, Node* node2)
{
    if (node2 == nullptr)
        return node1;
    if (node1 == nullptr)
        return node2;
    if (node1->priority > node2->priority) {
        node1->right = merge(node1->right, node2);
        node1->fix();
        return node1;
    }
    else {
        node2->left = merge(node1, node2->left);
        node2->fix();
        return node2;
    }
}

/******************************************************************************/

ImplicitTreap::Node::Node(const std::string& value)
        : value(value)
        , priority(rand())
        , left(nullptr)
        , right(nullptr)
        , treeSize(1)
{}



void ImplicitTreap::Node::fix()
{
    treeSize = 1;
    if (left != nullptr)
        treeSize += left->treeSize;
    if (right != nullptr)
        treeSize += right->treeSize;
}