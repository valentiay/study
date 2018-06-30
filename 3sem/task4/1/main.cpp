#include <iostream>
#include "biginteger.h"

int main() {
    BigInteger b1 = 10;
    BigInteger b2 = 4;
    std::cout << b1 + b2 << "\n";
    std::cout << b1 - b2 << "\n";
    std::cout << b1 * b2 << "\n";
    std::cout << b1 / b2 << "\n";
    std::cout << b1 % 7 << "\n";
    if (!b1) {
        std::cout << "yes";
    }
}