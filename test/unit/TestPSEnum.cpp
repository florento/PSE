//
//  TestPSRawEnum.cpp
//  testpse
//
//  Created by Florent Jacquemard on 05/04/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Scale.hpp"
#include "Ton.hpp"
#include "PSEnum.hpp"
#include "PSRawEnum.hpp"


//class TestPSRawEnum : public ::testing::Test
//{
//protected:
//
//    void SetUp() override
//    {
//        _enum = pse::PSRawEnum(0, 12); // empty
//        _enum.add();
//    }
//
//    void TearDown() override
//    {
//        //ASSERT_NE(sc, nullptr);
//        // delete sc;
//    }
//
//    // Measure* currentMeasure() { return mes; }
//    pse::PSRawEnum _enum;
//};


TEST(PSEnum, empty)
{
    pse::PSRawEnum e0(0, 0); // empty
    EXPECT_TRUE(e0.empty());
    EXPECT_FALSE(e0.open());
    EXPECT_EQ(e0.first(), 0);
    EXPECT_EQ(e0.stop(), 0);
    EXPECT_EQ(e0.size(), 0);
    EXPECT_FALSE(e0.inside(0));
    EXPECT_FALSE(e0.inside(1));
    EXPECT_TRUE(e0.outside(0));
}

TEST(PSEnum, open)
{
    pse::PSRawEnum e(0);
    EXPECT_FALSE(e.empty());
    EXPECT_TRUE(e.open());
    EXPECT_EQ(e.first(), 0);
    EXPECT_TRUE(e.inside(0));
    EXPECT_TRUE(e.inside(1));
    EXPECT_TRUE(e.inside(1000));
}

TEST(PSEnum, base)
{
    pse::PSRawEnum e(0, 12);
    EXPECT_FALSE(e.empty());
    EXPECT_FALSE(e.open());
    EXPECT_EQ(e.first(), 0);
    EXPECT_EQ(e.stop(), 12);
    EXPECT_TRUE(e.inside(0));
    EXPECT_TRUE(e.inside(1));
    EXPECT_TRUE(e.inside(11));
    EXPECT_FALSE(e.inside(12));
    EXPECT_TRUE(e.outside(1000));
}

TEST(PSEnum, count)
{
    pse::PSRawEnum e(0); // open
    
    // midi key, bar nb, simult
    e.add(36, 0, false); // C2
    e.add(35, 0, false); // B1
    e.add(36, 0, false); // C2
    e.add(38, 0, true);  // D2
    
    e.add(36, 1, false); // C2
    e.add(35, 1, false); // B1
    e.add(36, 1, false); // C2
    e.add(33, 1, false); // A1
    
    e.add(31, 2, false); // G1
    e.add(30, 2, false); // F#1
    e.add(31, 2, false); // G1
    e.add(28, 2, false); // E1
    
    e.add(29, 3, false); // F1
    e.add(26, 3, false); // D1
    e.add(23, 3, false); // B0
    e.add(19, 3, false); // G0
    
    e.add(24, 4, false); // C1
    e.add(23, 4, false); // B0
    e.add(24, 4, false); // C1
    e.add(21, 4, false); // A0

    e.add(19, 4, false); // G0
    e.add(16, 4, false); // E0
    e.add(17, 4, false); // F0
    e.add(14, 4, false); // D0

    e.add(12, 4, false); // C0

    e.rename(0, pse::NoteName::C);
    e.rename(1, pse::NoteName::B);
    e.rename(2, pse::NoteName::C);
    e.rename(3, pse::NoteName::D);
    
    e.rename(4, pse::NoteName::C);
    e.rename(5, pse::NoteName::B);
    e.rename(6, pse::NoteName::C);
    e.rename(7, pse::NoteName::A);
    
    e.rename(8, pse::NoteName::G);
    e.rename(9, pse::NoteName::F); // F#
    e.rename(10, pse::NoteName::G);
    e.rename(11, pse::NoteName::E);

    e.rename(12, pse::NoteName::F);
    e.rename(13, pse::NoteName::D);
    e.rename(14, pse::NoteName::B);
    e.rename(15, pse::NoteName::G);

    e.rename(16, pse::NoteName::C);
    e.rename(17, pse::NoteName::B);
    e.rename(18, pse::NoteName::C);
    e.rename(19, pse::NoteName::A);

    e.rename(20, pse::NoteName::G);
    e.rename(21, pse::NoteName::E);
    e.rename(22, pse::NoteName::F);
    e.rename(23, pse::NoteName::D);

    e.rename(24, pse::NoteName::C);
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 2);

    EXPECT_EQ(e.name(9), pse::NoteName::F);
    EXPECT_EQ(e.accidental(9), pse::Accid::Sharp);
    EXPECT_EQ(e.octave(9), 1);

    EXPECT_EQ(e.name(10), pse::NoteName::G);
    EXPECT_EQ(e.accidental(10), pse::Accid::Natural);
    EXPECT_EQ(e.octave(10), 1);

    EXPECT_EQ(e.name(24), pse::NoteName::C);
    EXPECT_EQ(e.accidental(24), pse::Accid::Natural);
    EXPECT_EQ(e.octave(24), 0);
    
    // pitch class, note index, pre, post
    EXPECT_EQ(e.count(0, 0, 10, 10), 4);  // C in window 0..9
    EXPECT_EQ(e.count(6, 0, 10, 10), 1);  // F# in window 0..9
    EXPECT_EQ(e.count(0, 12, 10, 10), 5); // C in window 2..21
    EXPECT_EQ(e.count(4, 12, 10, 10), 2); // E in window 2..21
    EXPECT_EQ(e.count(5, 12, 10, 10), 1); // F in window 2..21
    EXPECT_EQ(e.count(0, 24, 10, 10), 3); // C in window 14..24
 
    EXPECT_EQ(e.count(6, 0, 30, 30), 1);  // F# in whole
    EXPECT_EQ(e.count(7, 0, 30, 30), 4);  // G in whole
    EXPECT_EQ(e.count(11, 0, 30, 30), 4); // B in whole

    EXPECT_EQ(e.count(0, 16, 5, 5), 2);  // C in a window 11..20
    EXPECT_EQ(e.count(4, 16, 5, 5), 1);  // E in a window 11..20
    EXPECT_EQ(e.count(7, 16, 5, 5), 2);  // G in a window 11..20
    EXPECT_EQ(e.count(11, 16, 5, 5), 2); // B in a window 11..20
    EXPECT_EQ(e.count(6, 16, 5, 5), 0);  // F# in a window 11..20
}

TEST(PSEnum, rewritePassing_a)
{
    pse::PSRawEnum e(0); // open
    
    // midi key, bar nb, simult
    e.add(48, 0, false); // C3
    e.add(47, 0, false); // Cb3
    e.add(48, 0, false); // C3
    
    e.rename(0, pse::NoteName::C);
    e.rename(1, pse::NoteName::C);
    e.rename(2, pse::NoteName::C);
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::C);
    EXPECT_EQ(e.accidental(1), pse::Accid::Flat);
    EXPECT_EQ(e.octave(1), 3);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 3);

    e.rewritePassing();
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::B);
    EXPECT_EQ(e.accidental(1), pse::Accid::Natural);
    EXPECT_EQ(e.octave(1), 2);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 3);
}

TEST(PSEnum, rewritePassing_b)
{
    pse::PSRawEnum e(0); // open
    
    // midi key, bar nb, simult
    e.add(48, 0, false); // C3
    e.add(49, 0, false); // C#3
    e.add(48, 0, false); // C3
    
    e.rename(0, pse::NoteName::C);
    e.rename(1, pse::NoteName::C);
    e.rename(2, pse::NoteName::C);
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::C);
    EXPECT_EQ(e.accidental(1), pse::Accid::Sharp);
    EXPECT_EQ(e.octave(1), 3);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 3);

    e.rewritePassing();
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::D);
    EXPECT_EQ(e.accidental(1), pse::Accid::Flat);
    EXPECT_EQ(e.octave(1), 3);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 3);
}

TEST(PSEnum, rewritePassing_c)
{
    pse::PSRawEnum e(0); // open
    
    // midi key, bar nb, simult
    e.add(48, 0, false); // C3
    e.add(47, 0, false); // Cb3
    e.add(45, 0, false); // A2
    
    e.rename(0, pse::NoteName::C);
    e.rename(1, pse::NoteName::C);
    e.rename(2, pse::NoteName::A);
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::C);
    EXPECT_EQ(e.accidental(1), pse::Accid::Flat);
    EXPECT_EQ(e.octave(1), 3);

    EXPECT_EQ(e.name(2), pse::NoteName::A);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 2);

    e.rewritePassing();
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::B);
    EXPECT_EQ(e.accidental(1), pse::Accid::Natural);
    EXPECT_EQ(e.octave(1), 2);

    EXPECT_EQ(e.name(2), pse::NoteName::A);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 2);
}

TEST(PSEnum, rewritePassing_d)
{
    pse::PSRawEnum e(0); // open
    
    // midi key, bar nb, simult
    e.add(45, 0, false); // A2
    e.add(47, 0, false); // Cb3
    e.add(48, 0, false); // C3
    
    e.rename(0, pse::NoteName::A);
    e.rename(1, pse::NoteName::C);
    e.rename(2, pse::NoteName::C);
    
    EXPECT_EQ(e.name(0), pse::NoteName::A);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 2);

    EXPECT_EQ(e.name(1), pse::NoteName::C);
    EXPECT_EQ(e.accidental(1), pse::Accid::Flat);
    EXPECT_EQ(e.octave(1), 3);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 3);

    e.rewritePassing();
    
    EXPECT_EQ(e.name(0), pse::NoteName::A);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 2);

    EXPECT_EQ(e.name(1), pse::NoteName::B);
    EXPECT_EQ(e.accidental(1), pse::Accid::Natural);
    EXPECT_EQ(e.octave(1), 2);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 3);
}

TEST(PSEnum, rewritePassing_e)
{
    pse::PSRawEnum e(0); // open
    
    // midi key, bar nb, simult
    e.add(48, 0, false); // C3
    e.add(46, 0, false); // A#3
    e.add(45, 0, false); // A2
    
    e.rename(0, pse::NoteName::C);
    e.rename(1, pse::NoteName::A);
    e.rename(2, pse::NoteName::A);
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::A);
    EXPECT_EQ(e.accidental(1), pse::Accid::Sharp);
    EXPECT_EQ(e.octave(1), 2);

    EXPECT_EQ(e.name(2), pse::NoteName::A);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 2);

    e.rewritePassing();
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::B);
    EXPECT_EQ(e.accidental(1), pse::Accid::Flat);
    EXPECT_EQ(e.octave(1), 2);

    EXPECT_EQ(e.name(2), pse::NoteName::A);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 2);
}

TEST(PSEnum, rewritePassing_f)
{
    pse::PSRawEnum e(0); // open
    
    // midi key, bar nb, simult
    e.add(45, 0, false); // A2
    e.add(46, 0, false); // A#3
    e.add(48, 0, false); // C3
    
    e.rename(0, pse::NoteName::A);
    e.rename(1, pse::NoteName::A);
    e.rename(2, pse::NoteName::C);
    
    EXPECT_EQ(e.name(0), pse::NoteName::A);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 2);

    EXPECT_EQ(e.name(1), pse::NoteName::A);
    EXPECT_EQ(e.accidental(1), pse::Accid::Sharp);
    EXPECT_EQ(e.octave(1), 2);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 3);

    e.rewritePassing();
    
    EXPECT_EQ(e.name(0), pse::NoteName::A);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 2);

    EXPECT_EQ(e.name(1), pse::NoteName::B);
    EXPECT_EQ(e.accidental(1), pse::Accid::Flat);
    EXPECT_EQ(e.octave(1), 2);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 3);
}

TEST(PSEnum, rewritePassing_a4)
{
    pse::PSRawEnum e(0); // open
    
    // midi key, bar nb, simult
    e.add(48, 0, false); // C3
    e.add(48, 0, false); // C3    rewrite position
    e.add(47, 0, false); // Cb3
    e.add(48, 0, false); // C3
    e.add(50, 0, false); // D3

    e.rename(0, pse::NoteName::C);
    e.rename(1, pse::NoteName::C);
    e.rename(2, pse::NoteName::C);
    e.rename(3, pse::NoteName::C);
    e.rename(4, pse::NoteName::D);

    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::C);
    EXPECT_EQ(e.accidental(1), pse::Accid::Natural);
    EXPECT_EQ(e.octave(1), 3);

    EXPECT_EQ(e.name(2), pse::NoteName::C);
    EXPECT_EQ(e.accidental(2), pse::Accid::Flat);
    EXPECT_EQ(e.octave(2), 3);

    EXPECT_EQ(e.name(3), pse::NoteName::C);
    EXPECT_EQ(e.accidental(3), pse::Accid::Natural);
    EXPECT_EQ(e.octave(3), 3);

    EXPECT_EQ(e.name(4), pse::NoteName::D);
    EXPECT_EQ(e.accidental(4), pse::Accid::Natural);
    EXPECT_EQ(e.octave(4), 3);

    e.rewritePassing();
    
    EXPECT_EQ(e.name(0), pse::NoteName::C);
    EXPECT_EQ(e.accidental(0), pse::Accid::Natural);
    EXPECT_EQ(e.octave(0), 3);

    EXPECT_EQ(e.name(1), pse::NoteName::C);
    EXPECT_EQ(e.accidental(1), pse::Accid::Natural);
    EXPECT_EQ(e.octave(1), 3);

    EXPECT_EQ(e.name(2), pse::NoteName::B);
    EXPECT_EQ(e.accidental(2), pse::Accid::Natural);
    EXPECT_EQ(e.octave(2), 2);

    EXPECT_EQ(e.name(3), pse::NoteName::C);
    EXPECT_EQ(e.accidental(3), pse::Accid::Natural);
    EXPECT_EQ(e.octave(3), 3);

    EXPECT_EQ(e.name(4), pse::NoteName::D);
    EXPECT_EQ(e.accidental(4), pse::Accid::Natural);
    EXPECT_EQ(e.octave(4), 3);
}
