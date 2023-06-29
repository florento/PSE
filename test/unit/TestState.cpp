//
//  TestState.cpp
//  testpse
//
//  Created by Florent Jacquemard on 01/06/2023.
//

#include "gtest/gtest.h"

#include "PSState.hpp"
#include "Ton.hpp"
#include "PSState.hpp"


TEST(PSState, C_maj)
{
    pse::PSState s(pse::Ton(0, pse::ModeName::Major));
    
    EXPECT_EQ(s.accid(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::E), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::A), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::B), pse::Accid::Natural);
    pse::PSState s0(s);
    
    bool print = s.update(pse::NoteName::F, pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::E), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::A), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::B), pse::Accid::Natural);
    pse::PSState s1(s);
    EXPECT_FALSE(s == s0);
    EXPECT_EQ(s.dist(s0), 1);
    EXPECT_EQ(s0.dist(s), 1);
    EXPECT_EQ(print, true);

    print = s.update(pse::NoteName::F, pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::E), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::A), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::B), pse::Accid::Natural);
    EXPECT_TRUE(s == s1);
    EXPECT_EQ(print, false);
    
    print = s.update(pse::NoteName::F, pse::Accid::Natural);
    EXPECT_TRUE(s == s0);
    EXPECT_EQ(s.dist(s0), 0);
    EXPECT_EQ(s0.dist(s), 0);
    EXPECT_EQ(print, true);
}


TEST(PSState, C_min)
{
    const pse::Ton gton(-3, pse::ModeName::Minor);
    
    pse::PSState s(gton);
    EXPECT_EQ(s.accid(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(s.accid(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::A), pse::Accid::Flat);
    EXPECT_EQ(s.accid(pse::NoteName::B), pse::Accid::Flat);
    pse::PSState s0(s);
    
    bool print = s.update(pse::NoteName::B, pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(s.accid(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::A), pse::Accid::Flat);
    EXPECT_EQ(s.accid(pse::NoteName::B), pse::Accid::Natural);
    EXPECT_EQ(print, true);
    
    
    EXPECT_FALSE(s == s0);
    EXPECT_EQ(s.dist(s0), 1);
    EXPECT_EQ(s0.dist(s), 1);
}


TEST(PSState, Bb_min)
{
    pse::PSState s(pse::Ton(-5, pse::ModeName::Minor));
    
    EXPECT_EQ(s.accid(pse::NoteName::B), pse::Accid::Flat);
    EXPECT_EQ(s.accid(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::D), pse::Accid::Flat);
    EXPECT_EQ(s.accid(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(s.accid(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(s.accid(pse::NoteName::G), pse::Accid::Flat);
    EXPECT_EQ(s.accid(pse::NoteName::A), pse::Accid::Flat);
}


TEST(PSState, As_min)
{
    pse::PSState s(pse::Ton(7, pse::ModeName::Minor));
    
    EXPECT_EQ(s.accid(pse::NoteName::A), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::B), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::C), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::D), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::E), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::G), pse::Accid::Sharp);
    pse::PSState s0(s);
    
    s.update(pse::NoteName::G, pse::Accid::DoubleSharp);

    EXPECT_EQ(s.accid(pse::NoteName::A), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::B), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::C), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::D), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::E), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(s.accid(pse::NoteName::G), pse::Accid::DoubleSharp);
    
    EXPECT_FALSE(s == s0);
    EXPECT_EQ(s.dist(s0), 1);
    EXPECT_EQ(s0.dist(s), 1);
}
