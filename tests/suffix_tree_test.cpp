//
// Created by admin on 17/09/2021.
//

#include "gtest/gtest.h"
#include <suffix_tree.h>

TEST(SuffixTreeModulo2, test_eq) {
    SuffixTree st;
    EXPECT_EQ(st.modulo_2(2), 0);
}