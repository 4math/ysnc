//
// Created by admin on 17/09/2021.
//

#include "gtest/gtest.h"
#include <tokenizer.h>

TEST(TokenizerModulo15, test_eq) {
    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.modulo_5(15), 0);
}
