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
        
        EXPECT_EQ(pse::Fifths::pc(i), m%12);
    }
}
