#include <fstream>
#include <vector>
#include <iostream>
#include <list>
#include <string>

/******************************************************************************/

typedef char byte;
typedef bool bit;

/******************************************************************************/

using std::vector;
using std::list;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;

/******************************************************************************/

class ostreambit{
public:
    // Constructor, gets input stream
    ostreambit(ofstream & stream);


    // Writes a bit. Returns true, when byte is written
    bool    writeBit(bit value);

private:
    // Stores stream
    ofstream &  stream_;

    // Stores byte to be written
    byte        currentByte_;

    // Stores number of bits in current byte
    char        bitsWritten_;

};



class istreambit{
public:
    // Constructor, gets output stream
    istreambit(ifstream & stream);


    // Reads a bit. Returns false if stream is over
    bool    readBit(bit & value);

private:
    // Stores stream
    ifstream &  stream_;

    // Stores reflected byte to be read
    byte    currentByte_;

    // Stores number of bits read
    char    bitsRead_;

};

/******************************************************************************/

class Node{
public:

    Node();

    Node(byte value, long weight, Node * left, Node * right);


    byte    value;

    long    weight;


    Node *  left;

    Node *  right;

};


bool nodeComparator(Node * & L, Node * R);


// Builds Huffman tree
Node *  buildTree(vector <long> freq);

// Initializes symbol codes
void    getCodes(Node * node, long code, vector <long> & codes);

// Prints Huffman tree
void    printTree(Node * node, int depth, const vector <long> & codes);

// Deletes Huffman tree
void    deleteTree(Node * node);

// Counts length for each code
void    countCodeLengths(const vector <long> & codes,
                         vector <char> & codeLengths);

// Encodes file
void    Encode(ifstream & original, ofstream & compressed);

/******************************************************************************/

int main(){
    ifstream original("original.txt");
    ofstream compressed("encoded.txt");
    Encode(original, compressed);
    compressed.close();
    original.close();
}

/****************************bitstreams****************************************/

istreambit::istreambit(ifstream & stream):
    stream_(stream),
    bitsRead_(0)
{}



bool istreambit::readBit(bit & value){
    if(bitsRead_ == 0){
        byte tmp = 0;
        if (!stream_.get(tmp))
            return false;
        else {
            currentByte_ = 0;
            for (int i = 0; i < 8; i++){
                currentByte_ <<= 1;
                currentByte_ |= 1 & tmp;
                tmp >>= 1;
            }
        }
    }
    value = currentByte_ & 1;
    currentByte_ >>= 1;
    bitsRead_ = (bitsRead_ + 1) % 8;
    return true;
}



ostreambit::ostreambit(ofstream & stream):
    stream_(stream),
    bitsWritten_(0)
{}



bool ostreambit::writeBit(bit value){
    currentByte_ <<= 1;
    currentByte_ |= (value)?1:0;
    bitsWritten_++;
    if(bitsWritten_ == 8){
        stream_ << currentByte_;
        bitsWritten_ = 0;
        return true;
    }
    return false;
}

/******************************************************************************/

Node::Node():
    value(0),
    weight(0),
    left(nullptr),
    right(nullptr)
{}



Node::Node(byte value, long weight, Node * left, Node * right):
    value(value),
    weight(weight),
    left(left),
    right(right)
{}



bool nodeComparator(Node * & L, Node * R){
    return L->weight < R->weight;
}



Node * buildTree(vector<long> freq){
    list <Node *> list;
    for(int i = 0; i < 256; i++)
        if(freq[i] != 0){
            Node * node = new Node(i, freq[i], nullptr, nullptr);
            list.push_back(node);
        }

    while(list.size() > 1){
        list.sort(nodeComparator);
        Node * left = list.front();
        list.pop_front();
        Node * right = list.front();
        list.pop_front();
        Node * father = new Node(0, left->weight + right->weight, left, right);
        list.push_back(father);
    }
    Node * tree = list.front();
    return tree;
}



void getCodes(Node * node, long code, vector <long> & codes){
    if(node->left == nullptr){
        codes[node->value] = code;
        return;
    }

    getCodes(node->left, code << 1, codes);
    getCodes(node->right, (code << 1) + 1, codes);
}



void printTree(Node * node, int depth, const vector <long> & codes){
    if(node->left != nullptr){
        printTree(node->right, depth + 1, codes);
        printTree(node->left, depth + 1, codes);
        return;
    }
    for(int i = 0; i < depth*3; i++)
        cout << ' ';
    cout << node->value << " (";
    long code = codes[node->value];
    while(code != 1){
        cout << (code & 1);
        code >>= 1;
    }
    cout << ")\n";
}



void deleteTree(Node * node){
    if(node == nullptr){
        return;
    }
    deleteTree(node->right);
    deleteTree(node->left);
    delete node;
}



void countCodeLengths(const vector <long> & codes, vector <char> & codeLengths){
    for(int i = 0; i < 256; i++){
        codeLengths[i] = 0;
        if(codes[i] != 0){
            long code = codes[i];
            while(code != 1){
                codeLengths[i]++;
                code >>= 1;
            }
        }
    }
}



void Encode(ifstream & original, ofstream & compressed){
    // Stores frequencies vector
    vector <long> freq(256);

    // Reading file and counting frequencies
    string file;
    byte value = 0;
    while(!original.eof()){
        original.get(value);
        freq[value]++;
        file += value;
    }
    freq[value]--;  /* ??? */
    file.erase(file.length() - 1);

    // Stores codes of symbols. If code is ABCD, 1DCBA is stored
    vector <long> codes(256);

    Node * tree = buildTree(freq);  // Building Huffman tree
    getCodes(tree, 1, codes);       // Initializing codes
    deleteTree(tree);               // Deleting Huffman tree

    // Counting length of each code and maximum length of code
    vector <char> codeLengths(256);
    countCodeLengths(codes, codeLengths);
    int maxlen = 0;
    for(int i = 0; i < 255; i++)
        if(codeLengths[i] > maxlen)
            maxlen = codeLengths[i];

    // Countung number of unused bits int the rest of encoded file
    size_t encodedSize = 0;
    for(int i = 0; i < 256; i++)
        encodedSize = (encodedSize + freq[i] * codeLengths[i]) % 8;
    byte restBits = (8 - encodedSize % 8) % 8;

    // Counting number of bytes for storing one code
    byte bytesPerCode = (maxlen % 8 == 0)?(maxlen / 8):(maxlen / 8 + 1);

    ostreambit bout(compressed);

    /* Encoded file structure:
     * 1 byte storing number of unused bits in the rest of file
     *
     * Table:
     * For each symbol:
     * 1 byte storing coded symbol
     * bytesPerCode of bytes storing code
     *
     * Encoded data
     *
     * restBits of zeros */

    // Writing encoded file
    compressed << restBits;
    compressed << bytesPerCode;
    for(int i = 0; i < 256; i++)
        if(freq[i] != 0){
            long code = codes[i];
            int j = 0;
            compressed << byte(i);
            while(code != 1){
                bout.writeBit(code & 1);
                code >>= 1;
                j++;
            }
            for(; j < bytesPerCode * 8; j++)
                bout.writeBit(0);
        }
    for(size_t i = 0; i < file.length(); i++){
        long code = codes[file[i]];
        while(code != 1){
            bout.writeBit(code & 1);
            code >>= 1;
        }
    }
    for(int i = 0; i < restBits; i++)
        bout.writeBit(0);
}
