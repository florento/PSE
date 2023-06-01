//
//  TestMode.cpp
//  testpse
//
//  Created by Florent Jacquemard on 31/03/2023.
//

#include <stdio.h>

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "ModeName.hpp"
#include "Mode.hpp"


TEST(Mode, size)
{
    // pse::ModeName::Undef)
    EXPECT_EQ(pse::Mode().size(), 0);

    // diatonic modes
    EXPECT_EQ(pse::Mode(pse::ModeName::Major).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::Minor).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::MinorNat).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::MinorMel).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::Ionian).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::Dorian).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::Phrygian).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::Lydian).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::Mixolydian).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::Aeolian).size(), 7);
    EXPECT_EQ(pse::Mode(pse::ModeName::Locrian).size(), 7);

    EXPECT_EQ(pse::Mode(pse::ModeName::Major5).size(), 5);
    EXPECT_EQ(pse::Mode(pse::ModeName::Minor5).size(), 5);
    EXPECT_EQ(pse::Mode(pse::ModeName::MajorBlues).size(), 6);
    EXPECT_EQ(pse::Mode(pse::ModeName::MinorBlues).size(), 6);
    EXPECT_EQ(pse::Mode(pse::ModeName::Augmented).size(), 6);
    EXPECT_EQ(pse::Mode(pse::ModeName::Diminished).size(), 8);
    EXPECT_EQ(pse::Mode(pse::ModeName::Chromatic).size(), 12);
}

TEST(Mode, semitonDistance_major)
{
    pse::Mode m(pse::ModeName::Major);
    EXPECT_EQ(m.semitonDistance(0), 0);
    EXPECT_EQ(m.semitonDistance(1), 2);
    EXPECT_EQ(m.semitonDistance(2), 4);
    EXPECT_EQ(m.semitonDistance(3), 5);
    EXPECT_EQ(m.semitonDistance(4), 7);
    EXPECT_EQ(m.semitonDistance(5), 9);
    EXPECT_EQ(m.semitonDistance(6), 11);
}

TEST(Mode, semitonDistance_minorharm)
{
    pse::Mode m(pse::ModeName::Minor);
    EXPECT_EQ(m.semitonDistance(0), 0);
    EXPECT_EQ(m.semitonDistance(1), 2);
    EXPECT_EQ(m.semitonDistance(2), 3);
    EXPECT_EQ(m.semitonDistance(3), 5);
    EXPECT_EQ(m.semitonDistance(4), 7);
    EXPECT_EQ(m.semitonDistance(5), 8);
    EXPECT_EQ(m.semitonDistance(6), 11);
}

TEST(Mode, semitonDistance_chromatic)
{
    pse::Mode m(pse::ModeName::Chromatic);
    EXPECT_EQ(m.semitonDistance(0), 0);
    EXPECT_EQ(m.semitonDistance(1), 1);
    EXPECT_EQ(m.semitonDistance(2), 2);
    EXPECT_EQ(m.semitonDistance(3), 3);
    EXPECT_EQ(m.semitonDistance(4), 4);
    EXPECT_EQ(m.semitonDistance(5), 5);
    EXPECT_EQ(m.semitonDistance(6), 6);
    EXPECT_EQ(m.semitonDistance(7), 7);
    EXPECT_EQ(m.semitonDistance(8), 8);
    EXPECT_EQ(m.semitonDistance(9), 9);
    EXPECT_EQ(m.semitonDistance(10), 10);
    EXPECT_EQ(m.semitonDistance(11), 11);
}

TEST(Mode, nameDistance_major)
{
    pse::Mode m(pse::ModeName::Major);
    EXPECT_EQ(m.nameDistance(0), 0);
    EXPECT_EQ(m.nameDistance(1), 1);
    EXPECT_EQ(m.nameDistance(2), 2);
    EXPECT_EQ(m.nameDistance(3), 3);
    EXPECT_EQ(m.nameDistance(4), 4);
    EXPECT_EQ(m.nameDistance(5), 5);
    EXPECT_EQ(m.nameDistance(6), 6);
}

TEST(Mode, nameDistance_minorharm)
{
    pse::Mode m(pse::ModeName::Minor);
    EXPECT_EQ(m.nameDistance(0), 0);
    EXPECT_EQ(m.nameDistance(1), 1);
    EXPECT_EQ(m.nameDistance(2), 2);
    EXPECT_EQ(m.nameDistance(3), 3);
    EXPECT_EQ(m.nameDistance(4), 4);
    EXPECT_EQ(m.nameDistance(5), 5);
    EXPECT_EQ(m.nameDistance(6), 6);
}

TEST(Mode, nameDistance_chromatic)
{
    pse::Mode m(pse::ModeName::Chromatic);
    EXPECT_EQ(m.nameDistance(0), 0);
    EXPECT_EQ(m.nameDistance(1), 1);
    EXPECT_EQ(m.nameDistance(2), 1);
    EXPECT_EQ(m.nameDistance(3), 2);
    EXPECT_EQ(m.nameDistance(4), 2);
    EXPECT_EQ(m.nameDistance(5), 3);
    EXPECT_EQ(m.nameDistance(6), 3);
    EXPECT_EQ(m.nameDistance(7), 4);
    EXPECT_EQ(m.nameDistance(8), 5);
    EXPECT_EQ(m.nameDistance(9), 5);
    EXPECT_EQ(m.nameDistance(10), 6);
    EXPECT_EQ(m.nameDistance(11), 6);
}
