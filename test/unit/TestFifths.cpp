//
//  TestFifths.cpp
//  testpse
//
//  Created by Florent Jacquemard on 11/05/2023.
//

#include "gtest/gtest.h"

#include "Fifths.hpp"
#include "MidiNum.hpp"

TEST(Fifths, pitch_class)
{
    for (int i = -15; i <= 19; ++i)
    {
        unsigned int m =
        pse::MidiNum::to_midi(pse::Fifths::name(i), pse::Fifths::accid(i), 0);
        ASSERT_LE(0, pse::Fifths::pc(i));
        ASSERT_LE(pse::Fifths::pc(i), 128);
        EXPECT_EQ(pse::Fifths::pc(i), m%12);
    }
}
