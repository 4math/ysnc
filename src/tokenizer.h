//
// Created by admin on 17/09/2021.
//

#ifndef YSNC_TOKENIZER_H
#define YSNC_TOKENIZER_H

#include <iostream>

class Tokenizer {
public:
    static void display() { std::cout << "I am a tokenizer!\n"; }

    [[nodiscard]] int32_t modulo_5(int32_t x) const { return x % 5; }
};

#endif //YSNC_TOKENIZER_H
