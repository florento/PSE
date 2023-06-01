//
//  TestKeyFifth.cpp
//  testpse
//
//  Created by Florent Jacquemard on 01/06/2023.
//

#include "gtest/gtest.h"

//#include "Fifths.hpp"
#include "KeyFifth.hpp"


TEST(KeyFifth, key_maj)
{
    EXPECT_EQ(pse::KeyFifth(0, true).fifths(),   0);
    EXPECT_EQ(pse::KeyFifth(1, true).fifths(),  -5); // alt 7
    EXPECT_EQ(pse::KeyFifth(2, true).fifths(),   2);
    EXPECT_EQ(pse::KeyFifth(3, true).fifths(),  -3);
    EXPECT_EQ(pse::KeyFifth(4, true).fifths(),   4);
    EXPECT_EQ(pse::KeyFifth(5, true).fifths(),  -1);
    EXPECT_EQ(pse::KeyFifth(6, true).fifths(),   6); // alt -6
    EXPECT_EQ(pse::KeyFifth(7, true).fifths(),   1);
    EXPECT_EQ(pse::KeyFifth(8, true).fifths(),  -4);
    EXPECT_EQ(pse::KeyFifth(9, true).fifths(),   3);
    EXPECT_EQ(pse::KeyFifth(10, true).fifths(), -2);
    EXPECT_EQ(pse::KeyFifth(11, true).fifths(),  5); // alt -7
}

TEST(KeyFifth, key_min)
{
    EXPECT_EQ(pse::KeyFifth(0, false).fifths(),  -3);
    EXPECT_EQ(pse::KeyFifth(1, false).fifths(),   4);
    EXPECT_EQ(pse::KeyFifth(2, false).fifths(),  -1);
    EXPECT_EQ(pse::KeyFifth(3, false).fifths(),   6); // alt -6
    EXPECT_EQ(pse::KeyFifth(4, false).fifths(),   1);
    EXPECT_EQ(pse::KeyFifth(5, false).fifths(),  -4);
    EXPECT_EQ(pse::KeyFifth(6, false).fifths(),   3);
    EXPECT_EQ(pse::KeyFifth(7, false).fifths(),  -2);
    EXPECT_EQ(pse::KeyFifth(8, false).fifths(),   5); // alt -7
    EXPECT_EQ(pse::KeyFifth(9, false).fifths(),   0);
    EXPECT_EQ(pse::KeyFifth(10, false).fifths(), -5); // alt 7
    EXPECT_EQ(pse::KeyFifth(11, false).fifths(),  2);
}
