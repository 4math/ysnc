//
// Created by admin on 14/09/2021.
//

#ifndef YSNC_SUFFIX_TREE_H
#define YSNC_SUFFIX_TREE_H

#include <iostream>

class SuffixTree {
public:
    static void display() { std::cout << "I am a SuffixTree!\n"; }

    [[nodiscard]] int32_t modulo_2(int32_t x) const { return x % 2; }
};

#endif //YSNC_SUFFIX_TREE_H
