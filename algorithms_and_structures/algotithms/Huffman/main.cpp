#include "Huffman.h"

/******************************************************************************/

int main(){
    ifstream original("original.txt");
    ofstream compressed1("encoded.txt");
    IInputStream originalI(original);
    IOutputStream compressed1I(compressed1);
    Encode(originalI, compressed1I);
    compressed1.close();
    original.close();

    ifstream compressed2("encoded.txt");
    ofstream decoded("decoded.txt");
    IInputStream compressed2I(compressed2);
    IOutputStream decodedI(decoded);
    Decode(compressed2I, decodedI);
    compressed2.close();
    decoded.close();

    original.open("original.txt");
    compressed2.open("decoded.txt");
    while(!original.eof()){
        byte a, b;
        original.get(a);
        compressed2.get(b);
        if(a != b)
            std::cerr << "Doesn't match \n";
    }
    cout << "Ready \n";
}