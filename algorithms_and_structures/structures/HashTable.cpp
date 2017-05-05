#include <iostream>

using std::cin;
using std::cout;
using std::string;


// HashTable v0.0.2
class HashTable{
public:
    // Default constructor
    HashTable();

    // Destructor
    ~HashTable();

    // Inserts string. Returns true on success and false otherwise
    bool        insert(string key);

    // Removes string. Returns true on success and false otherwise
    bool        remove(string key);


    // Returns true if table contains string and false otherwise
    bool        contains(string key) const;

    // Prints table
    void        print() const;

private:
    struct Node{
        // Constructs node with data = key
        Node(string key);


        string  data;

        bool    deleted;
    };

    // Rehashes table
    void        rehash();

    // Returns hash for key
    int         hash(string key) const;


    int         bufferSize_;

    int         size_;

    Node **      nodes_;
};

/******************************************************************************/

/****************************HASHTABLE*****************************************/

// Public:

HashTable::HashTable():
        bufferSize_(8),
        size_(0)
{
    nodes_ = new Node*[bufferSize_];
    for(size_t i = 0; i < bufferSize_; i++)
        nodes_[i] = nullptr;
}



HashTable::~HashTable(){
    for (int i = 0; i < bufferSize_; ++i)
        if (nodes_[i] != nullptr)
            delete nodes_[i];
    delete[] nodes_;
}



bool HashTable::insert(string key){
    if(contains(key))
        return false;

    if(3*bufferSize_ <= 4*(size_ + 1))
        rehash();

    int hashed = hash(key);
    int i = 0;

    while(i < bufferSize_){
        if(nodes_[hashed] == nullptr){
            nodes_[hashed] = new Node(key);
            size_++;
            return true;
        }
        else if(nodes_[hashed]->deleted){
            nodes_[hashed]->data = key;
            nodes_[hashed]->deleted = false;
            size_++;
            return true;
        }
        i++;
        hashed = (hashed + i) % bufferSize_;
    }
    return false;
}



bool HashTable::contains(string key) const{
    int hashed = hash(key);
    int i = 0;
    while(nodes_[hashed] != nullptr && i < bufferSize_){
        if(!nodes_[hashed]->deleted && nodes_[hashed]->data.compare(key) == 0)
            return true;
        i++;
        hashed = (hashed + i) % bufferSize_;
    }
    return false;
}



bool HashTable::remove(string key){
    if(!contains(key))
        return false;

    int hashed = hash(key);
    int i = 0;

    while(i < bufferSize_){
        if(!nodes_[hashed]->deleted && nodes_[hashed]->data.compare(key) == 0){
            nodes_[hashed]->deleted = true;
            size_--;
            return true;
        }
        i++;
        hashed = (hashed + i) % bufferSize_;
    }
    return false;
}




void HashTable::print() const{
    cout << "Alfa: " << (double) size_ / bufferSize_
         << "; Size: " << size_
         << "; BufferSize: " << bufferSize_ << '\n';
    for(int i = 0; i < bufferSize_; i++)
        if(nodes_[i] == nullptr)
            cout << "NULL \n";
        else
            cout << "KEY: " << nodes_[i]->data
                 << "; DELETED: " << nodes_[i]->deleted
                 << "; ADDR: " << nodes_[i] << '\n';
}

// Private:

void HashTable::rehash(){
    Node ** tmp = nodes_;
    bufferSize_ *= 2;
    size_ = 0;
    nodes_ = new Node*[bufferSize_];

    for(int i = 0; i < bufferSize_; i++)
        nodes_[i] = nullptr;

    for(int i = 0; i < bufferSize_ / 2; i++)
        if(tmp[i] != nullptr){
            if(!tmp[i]->deleted)
                insert(tmp[i]->data);
            delete tmp[i];
        }

    delete[] tmp;
}



int HashTable::hash(string key) const{
    int hashed = 0;
    for (size_t i = 0; i < key.length(); i++) {
        hashed = (hashed + 127 * key[i]) % bufferSize_;
    }
    return hashed;
}

/****************************HASHTABLE::NODE***********************************/

HashTable::Node::Node(string key) :
        data(key),
        deleted(false)
{}
