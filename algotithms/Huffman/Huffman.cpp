#include "Huffman.h"

/****************************bitstreams****************************************/

bitistream::bitistream(IInputStream & stream):
        stream_(stream),
        bitsRead_(0)
{}



bool bitistream::read(bit & value){
    if(bitsRead_ == 0){
        byte tmp = 0;
        if (!stream_.Read(tmp))
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



bitostream::bitostream(IOutputStream & stream):
        stream_(stream),
        bitsWritten_(0)
{}



bool bitostream::write(bit value){
    currentByte_ <<= 1;
    currentByte_ |= (value)?1:0;
    bitsWritten_++;
    if(bitsWritten_ == 8){
        stream_.Write(currentByte_);
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



Node * buildTree(map <byte, long> freq){
    list <Node *> list;
    for(map<byte, long>::iterator i = freq.begin(); i != freq.end(); i++)
        if(i->second != 0){
            Node * node = new Node(i->first, i->second, nullptr, nullptr);
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



void getCodes(Node * node, long code, map <byte, long> & codes){
    if(node->left == nullptr){
        long result = 1;
        while(code != 1){
            result <<= 1;
            result |= code & 1;
            code >>= 1;
        }
        codes[node->value] = result;
        return;
    }

    getCodes(node->left, code << 1, codes);
    getCodes(node->right, (code << 1) + 1, codes);
}



void printTree(Node * node, int depth, const map <byte, long> & codes){
    if(node->left != nullptr){
        printTree(node->right, depth + 1, codes);
        for(int i = 0; i < depth*3; i++)
            cout << ' ';
        cout << 0 << '\n';
        printTree(node->left, depth + 1, codes);
        return;
    }
    for(int i = 0; i < depth*3; i++)
        cout << ' ';
    cout << node->value << " (";
    long code = codes.at(node->value);
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



void countCodeLengths(const map <byte, long> & codes,
                      map <byte, char> & codeLengths){
    for(map<byte, long>::const_iterator i = codes.begin();
        i != codes.end(); i++){
        codeLengths[i->first] = 0;
        if(i->second != 0){
            long code = i->second;
            while(code != 1){
                codeLengths[i->first]++;
                code >>= 1;
            }
        }
    }
}



void fixTree(Node * node){
    if(node == nullptr)
        return;

    if(node->left != nullptr && node->right == nullptr){
        deleteTree(node->left);
        node->left = nullptr;
        return;
    }

    fixTree(node->left);
    fixTree(node->right);
}



void Encode(IInputStream & original, IOutputStream & compressed){
    // Stores frequencies vector
    map <byte, long> freq;

    // Reading file and counting frequencies
    string file;
    byte value = 0;
    while(original.Read(value)){
        freq[value]++;
        file += value;
    }
    freq[value]--;  /* ??? */
    file.erase(file.length() - 1);

    // Stores codes of symbols. If code is ABCD, 1DCBA is stored
    map <byte, long> codes;

    Node * tree = buildTree(freq);  // Building Huffman tree
    getCodes(tree, 1, codes);       // Initializing codes
    deleteTree(tree);               // Deleting Huffman tree

    // Counting length of each code and maximum length of code
    map <byte, char> codeLengths;
    countCodeLengths(codes, codeLengths);
    char maxlen = 0;
    for(map<byte, char>::iterator i = codeLengths.begin();
        i != codeLengths.end(); i++)
        if(i->second > maxlen)
            maxlen = i->second;

    // Countung number of unused bits int the rest of encoded file
    size_t encodedSize = 0;
    byte alphabetSize = 0;
    for(map<byte, long>::iterator i = freq.begin(); i != freq.end(); i++){
        if(i->second != 0)
            alphabetSize++;
        encodedSize = (encodedSize + i->second * codeLengths[i->first]) % 8;
    }
    byte restBits = 8 - encodedSize;

    // Counting number of bytes for storing one code
    byte bytesPerCode = (maxlen % 8 == 0)?(maxlen / 8):(maxlen / 8 + 1);

    bitostream bout(compressed);

    /* Encoded file structure:
     * 1 byte storing number of unused bits in the rest of file
     *
     * 1 byte storing number of bytes per code
     *
     * 1 byte storing size of alphabet
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
    compressed.Write(restBits);
    compressed.Write(bytesPerCode);
    compressed.Write(alphabetSize);
    for(map<byte, long>::iterator i = codes.begin(); i != codes.end(); i++)
        if(freq[i->first] != 0){
            long code = i->second;
            int j = 0;
            compressed.Write(byte(i->first));
            while(code != 1){
                bout.write(code & 1);
                code >>= 1;
                j++;
            }
            for(; j < bytesPerCode * 8; j++)
                bout.write(0);
        }
    for(size_t i = 0; i < file.length(); i++){
        long code = codes[file[i]];
        while(code != 1){
            bout.write(code & 1);
            code >>= 1;
        }
    }
    for(int i = 0; i < restBits; i++)
        bout.write(0);
}



void Decode(IInputStream & compressed, IOutputStream & decoded){
    // Reading information about encoded file
    byte restBits, bytesPerCode, tmp;
    compressed.Read(restBits);
    compressed.Read(bytesPerCode);
    compressed.Read(tmp);
    unsigned alphabetSize = (unsigned char) tmp;
    if(alphabetSize == 0)
        alphabetSize = 256;

    // Building Huffman tree
    Node * tree = new Node(0, 0, nullptr, nullptr);
    bitistream bin(compressed);
    for(int i = 0; i < alphabetSize; i++){
        byte symbol;
        Node * node = tree;
        compressed.Read(symbol);
        for(int j = 0; j < 8 * bytesPerCode; j++){
            bit bit1;
            bin.read(bit1);
            if(bit1 == 0){
                if(node->left == nullptr)
                    node->left = new Node(symbol, 0, nullptr, nullptr);
                node = node->left;
            }
            else{
                if(node->right == nullptr)
                    node->right = new Node(symbol, 0, nullptr, nullptr);
                node = node->right;
            }
        }
    }
    fixTree(tree);

    // Reading file
    string file;
    byte symbol;
    while(compressed.Read(symbol)){
        file += symbol;
    }
    file.erase(file.length() - 1);

    // Writing decoded file
    Node * node = tree;
    for(int i = 0; i < file.length() - 1; i++){
        for(int j = 7; j >= 0; j--){
            if(((file[i] >> j) & 1) == 0)
                node = node->left;
            else
                node = node->right;
            if(node->left == nullptr){
                decoded.Write(node->value);
                node = tree;
            }
        }
    }
    for(int j = 7; j >= restBits; j--){
        if(((file[file.length() - 1] >> j) & 1) == 0)
            node = node->left;
        else
            node = node->right;
        if(node->left == nullptr){
            decoded.Write(node->value);
            node = tree;
        }
    }

    deleteTree(tree);
}