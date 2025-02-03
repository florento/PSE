//
//  TestRewritePassing.cpp
//  testpse
//
//  Created by Florent Jacquemard on 07/06/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "Accid.hpp"
#include "ModeName.hpp"
#include "Scale.hpp"
#include "Ton.hpp"
#include "PSEnum.hpp"
#include "PSRawEnum.hpp"


TEST(RewritePassing, rewritePassing_a)
{
    pse::PSRawEnum e(0, 0); // empty, not open
    
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

TEST(RewritePassing, rewritePassing_b)
{
    pse::PSRawEnum e(0, 0); // empty, not open

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

TEST(RewritePassing, rewritePassing_c)
{
    pse::PSRawEnum e(0, 0); // empty, not open

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

TEST(RewritePassing, rewritePassing_d)
{
    pse::PSRawEnum e(0, 0); // empty, not open

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

TEST(RewritePassing, rewritePassing_e)
{
    pse::PSRawEnum e(0, 0); // empty, not open

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

TEST(RewritePassing, rewritePassing_f)
{
    pse::PSRawEnum e(0, 0); // empty, not open

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

TEST(RewritePassing, rewritePassing_a4)
{
    pse::PSRawEnum e(0, 0); // empty, not open

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
