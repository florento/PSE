//
//  TestScale.cpp
//  pse
//
//  Created by Florent Jacquemard on 31/03/2023.
//

#include "gtest/gtest.h"

#include "Scale.hpp"
#include "Ton.hpp"


TEST(Scale, major)
{
    EXPECT_EQ(log2floor(Rational(1)), 0);
    EXPECT_EQ(log2floor(Rational(3, 2)), 0);
    EXPECT_EQ(log2floor(Rational(5, 3)), 0);
}

