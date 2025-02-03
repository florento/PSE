//
//  TestEnharmonic.cpp
//  testpse
//
//  Created by Florent Jacquemard on 05/04/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "Accid.hpp"
#include "MidiNum.hpp"
#include "Enharmonic.hpp"

// relies on the correctness of MidiNum::pitchClass
TEST(Enharmonic, correctness)
{
    // for all pitch class
    for (int c = 0; c < 12; ++c)
    {
        // for all 3 defined cases
        for (int i = 0; i < 3; ++i)
        {
            enum pse::NoteName n = pse::Enharmonics::name(c, i);
            enum pse::Accid a = pse::Enharmonics::accid(c, i);
            EXPECT_EQ(n == pse::NoteName::Undef, a == pse::Accid::Undef);
            if (n != pse::NoteName::Undef)
                EXPECT_EQ(pse::MidiNum::pitchClass(n, a), c);
        }
    }
}

// relies on the correctness of MidiNum::accid
TEST(Enharmonic, completeness)
{
    // for all pitch class
    for (int c = 0; c < 12; ++c)
    {
        // for all note name
        for (int i = 0; i < 7; ++i)
        {
            enum pse::NoteName n = pse::NoteName(i);
            enum pse::Accid a = pse::MidiNum::accid(c, n);
            // n a is a possible spelling of c without triple sharp or flat
            if (a != pse::Accid::Undef &&
                a != pse::Accid::TripleSharp &&
                a != pse::Accid::TripleFlat)
            {
                //std::cout << c << ' ' << n << a << std::endl;
                EXPECT_TRUE(((pse::Enharmonics::name(c, 0) == n) &&
                             (pse::Enharmonics::accid(c, 0) == a)) ||
                            ((pse::Enharmonics::name(c, 1) == n) &&
                             (pse::Enharmonics::accid(c, 1) == a)) ||
                            ((pse::Enharmonics::name(c, 2) == n) &&
                             (pse::Enharmonics::accid(c, 2) == a)));
            }
        }
    }
}
