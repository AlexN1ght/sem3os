#include "Vector.hpp"
#include <iostream>

int main() {
    vector* vec = vector_create();
    char com = '0';
    int pos, val;
    while(1) {
        std::cin >> com;
        switch(com) {
            case 'p':
                std::cin >> val;
                vector_push_back(vec, val);
                break;
            case 'c':
                std::cin >> pos >> val;
                vector_at(vec, pos) = val;
                break;
            case 'o':
                std::cin >> pos;
                std::cout << vector_at(vec, pos) << '\n';
                break;
            case 'q':
                vector_destroy(&vec);
                return 0;
                break;
            default:
                std::cout << "err\n";
                break;
        }
    }
}
