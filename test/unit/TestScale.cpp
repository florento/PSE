//
//  TestScale.cpp
//  pse
//
//  Created by Florent Jacquemard on 31/03/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "ModeName.hpp"
#include "Scale.hpp"
#include "Ton.hpp"


TEST(Scale, C_maj)
{
    // tonic: pitch class 0 = C
    pse::Scale s(pse::ModeName::Major, 0, pse::NoteName::C);

    // key sig 0. mode maj = see table Ton::KEYS
    pse::Ton t(0, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(6));
}

TEST(Scale, Csharp_maj)
{
    pse::Scale s(pse::ModeName::Major, 1, pse::NoteName::C);
    pse::Ton t(7, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(6));
}

TEST(Scale, Dflat_maj)
{
    pse::Scale s(pse::ModeName::Major, 1, pse::NoteName::D);
    pse::Ton t(-5, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(6));
}


