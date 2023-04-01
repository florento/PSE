//
//  TestAccid.cpp
//  testpse
//
//  Created by Florent Jacquemard on 01/04/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "Accidental.hpp"


TEST(Accid, defined)
{
    EXPECT_FALSE(defined(pse::Accid::Undef));
    EXPECT_TRUE(defined(pse::Accid::TripleSharp));
    EXPECT_TRUE(defined(pse::Accid::DoubleSharp));
    EXPECT_TRUE(defined(pse::Accid::ThreeQuartersSharp));
    EXPECT_TRUE(defined(pse::Accid::Sharp));
    EXPECT_TRUE(defined(pse::Accid::QuarterSharp));
    EXPECT_TRUE(defined(pse::Accid::Natural));
    EXPECT_TRUE(defined(pse::Accid::QuarterFlat));
    EXPECT_TRUE(defined(pse::Accid::Flat));
    EXPECT_TRUE(defined(pse::Accid::ThreeQuartersFlat));
    EXPECT_TRUE(defined(pse::Accid::DoubleFlat));
    EXPECT_TRUE(defined(pse::Accid::TripleFlat));
}

TEST(Accid, natural)
{
    EXPECT_FALSE(natural(pse::Accid::Undef));
    EXPECT_FALSE(natural(pse::Accid::TripleSharp));
    EXPECT_FALSE(natural(pse::Accid::DoubleSharp));
    EXPECT_FALSE(natural(pse::Accid::ThreeQuartersSharp));
    EXPECT_FALSE(natural(pse::Accid::Sharp));
    EXPECT_FALSE(natural(pse::Accid::QuarterSharp));
    EXPECT_TRUE(natural(pse::Accid::Natural));
    EXPECT_FALSE(natural(pse::Accid::QuarterFlat));
    EXPECT_FALSE(natural(pse::Accid::Flat));
    EXPECT_FALSE(natural(pse::Accid::ThreeQuartersFlat));
    EXPECT_FALSE(natural(pse::Accid::DoubleFlat));
    EXPECT_FALSE(natural(pse::Accid::TripleFlat));
}

TEST(Accid, sharp)
{
    EXPECT_FALSE(sharp(pse::Accid::Undef));
    EXPECT_TRUE(sharp(pse::Accid::TripleSharp));
    EXPECT_TRUE(sharp(pse::Accid::DoubleSharp));
    EXPECT_TRUE(sharp(pse::Accid::ThreeQuartersSharp));
    EXPECT_TRUE(sharp(pse::Accid::Sharp));
    EXPECT_TRUE(sharp(pse::Accid::QuarterSharp));
    EXPECT_FALSE(sharp(pse::Accid::Natural));
    EXPECT_FALSE(sharp(pse::Accid::QuarterFlat));
    EXPECT_FALSE(sharp(pse::Accid::Flat));
    EXPECT_FALSE(sharp(pse::Accid::ThreeQuartersFlat));
    EXPECT_FALSE(sharp(pse::Accid::DoubleFlat));
    EXPECT_FALSE(sharp(pse::Accid::TripleFlat));
}

TEST(Accid, flat)
{
    EXPECT_FALSE(flat(pse::Accid::Undef));
    EXPECT_FALSE(flat(pse::Accid::TripleSharp));
    EXPECT_FALSE(flat(pse::Accid::DoubleSharp));
    EXPECT_FALSE(flat(pse::Accid::ThreeQuartersSharp));
    EXPECT_FALSE(flat(pse::Accid::Sharp));
    EXPECT_FALSE(flat(pse::Accid::QuarterSharp));
    EXPECT_FALSE(flat(pse::Accid::Natural));
    EXPECT_TRUE(flat(pse::Accid::QuarterFlat));
    EXPECT_TRUE(flat(pse::Accid::Flat));
    EXPECT_TRUE(flat(pse::Accid::ThreeQuartersFlat));
    EXPECT_TRUE(flat(pse::Accid::DoubleFlat));
    EXPECT_TRUE(flat(pse::Accid::TripleFlat));
}


