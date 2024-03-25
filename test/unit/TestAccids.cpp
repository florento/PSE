//
//  TestAccids.cpp
//  testpse
//
//  Created by Florent Jacquemard on 22/03/2024.
//

#include "gtest/gtest.h"

#include "Accids.hpp"



TEST(TestAccids, undef)
{
    pse::accids_t _UU = pse::Accids::encode();

    EXPECT_TRUE(pse::Accids::zero(_UU));
    EXPECT_FALSE(pse::Accids::single(_UU));
    EXPECT_FALSE(pse::Accids::two(_UU));
    EXPECT_EQ(pse::Accids::first(_UU), pse::Accid::Undef);
    EXPECT_EQ(pse::Accids::second(_UU), pse::Accid::Undef);
    EXPECT_FALSE(pse::Accids::contained(pse::Accid::Natural, _UU));
}

TEST(TestAccids, unary)
{
    pse::accids_t _f_ = pse::Accids::encode(pse::Accid::Flat);
    // pse::accids_t _n_ = pse::Accids::encode(pse::Accid::Natural);
    // pse::accids_t _s_ = pse::Accids::encode(pse::Accid::Sharp);
    
    EXPECT_FALSE(pse::Accids::zero(_f_));
    EXPECT_TRUE(pse::Accids::single(_f_));
    EXPECT_FALSE(pse::Accids::two(_f_));
    EXPECT_EQ(pse::Accids::first(_f_), pse::Accid::Flat);
    EXPECT_EQ(pse::Accids::second(_f_), pse::Accid::Undef);
    EXPECT_TRUE(pse::Accids::contained(pse::Accid::Flat, _f_));
    EXPECT_FALSE(pse::Accids::contained(pse::Accid::Natural, _f_));
}

TEST(TestAccids, flatnat)
{
    pse::accids_t _fn = pse::Accids::encode(pse::Accid::Flat,
                                            pse::Accid::Natural);
    
    EXPECT_FALSE(pse::Accids::zero(_fn));
    EXPECT_FALSE(pse::Accids::single(_fn));
    EXPECT_TRUE(pse::Accids::two(_fn));
    EXPECT_EQ(pse::Accids::first(_fn), pse::Accid::Flat);
    EXPECT_EQ(pse::Accids::second(_fn), pse::Accid::Natural);
    EXPECT_TRUE(pse::Accids::contained(pse::Accid::Flat, _fn));
    EXPECT_TRUE(pse::Accids::contained(pse::Accid::Natural, _fn));
}

TEST(TestAccids, natsharp)
{
    pse::accids_t _ns = pse::Accids::encode(pse::Accid::Natural,
                                            pse::Accid::Sharp);
    
    EXPECT_FALSE(pse::Accids::zero(_ns));
    EXPECT_FALSE(pse::Accids::single(_ns));
    EXPECT_TRUE(pse::Accids::two(_ns));
    EXPECT_EQ(pse::Accids::first(_ns), pse::Accid::Natural);
    EXPECT_EQ(pse::Accids::second(_ns), pse::Accid::Sharp);
    EXPECT_TRUE(pse::Accids::contained(pse::Accid::Natural, _ns));
    EXPECT_TRUE(pse::Accids::contained(pse::Accid::Sharp, _ns));
    EXPECT_FALSE(pse::Accids::contained(pse::Accid::Flat, _ns));
    EXPECT_FALSE(pse::Accids::contained(pse::Accid::DoubleFlat, _ns));
}

TEST(TestAccids, sharpdoublesharp)
{
    pse::accids_t _sS = pse::Accids::encode(pse::Accid::Sharp,
                                            pse::Accid::DoubleSharp);
    
    EXPECT_FALSE(pse::Accids::zero(_sS));
    EXPECT_FALSE(pse::Accids::single(_sS));
    EXPECT_TRUE(pse::Accids::two(_sS));
    EXPECT_EQ(pse::Accids::first(_sS), pse::Accid::Sharp);
    EXPECT_EQ(pse::Accids::second(_sS), pse::Accid::DoubleSharp);
    EXPECT_TRUE(pse::Accids::contained(pse::Accid::Sharp, _sS));
    EXPECT_TRUE(pse::Accids::contained(pse::Accid::DoubleSharp, _sS));
    EXPECT_FALSE(pse::Accids::contained(pse::Accid::Flat, _sS));
    EXPECT_FALSE(pse::Accids::contained(pse::Accid::Natural, _sS));
}

//TEST(TestAccids, doubleflatflat)
//{
//    pse::accids_t _Ff = pse::Accids::encode(pse::Accid::DoubleFlat,
//                                            pse::Accid::Flat);
// 
//}
