#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <fstream>
#include <map>
#include <iostream>
#include <list>
#include <string>

/******************************************************************************/

typedef char byte;
typedef bool bit;

/******************************************************************************/

using std::map;
using std::list;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;

/******************************************************************************/

class IInputStream {
public:
    IInputStream(ifstream & fin):
            fin_(fin)
    {}
    // Возвращает false, если поток закончился
    bool Read(byte& value){
        if(fin_.eof())
            return false;
        fin_.get(value);
        return true;
    }

private:
    ifstream & fin_;
};

class IOutputStream {
public:
    IOutputStream(ofstream & fout):
            fout_(fout)
    {}
    void Write(byte value){
        fout_ << value;
    }

private:
    ofstream & fout_;
};


/******************************************************************************/

class bitostream{
public:
    // Constructor, gets input stream
    bitostream(IOutputStream & stream);


    // Writes a bit. Returns true, when byte is written
    bool    write(bit value);

private:
    // Stores stream
    IOutputStream &  stream_;

    // Stores byte to be written
    byte        currentByte_;

    // Stores number of bits in current byte
    char        bitsWritten_;

};



class bitistream{
public:
    // Constructor, gets output stream
    bitistream(IInputStream & stream);


    // Reads a bit. Returns false if stream is over
    bool    read(bit & value);

private:
    // Stores stream
    IInputStream &  stream_;

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
Node *  buildTree(map <byte, long> freq);

// Initializes symbol codes
void    getCodes(Node * node, long code, map <byte, long> & codes);

// Prints Huffman tree
void    printTree(Node * node, int depth, const map <byte, long> & codes);

// Deletes Huffman tree
void    deleteTree(Node * node);

// Counts length for each code
void    countCodeLengths(const map <byte, long> & codes,
                         map <byte, char> & codeLengths);

// Deleting extra nodes in decoded tree
void    fixTree(Node * node);

// Encodes file
void    Encode(IInputStream & original, IOutputStream & compressed);

// Decodes file
void    Decode(IInputStream & compressed, IOutputStream & decoded);


#endif //HUFFMAN_HUFFMAN_H
