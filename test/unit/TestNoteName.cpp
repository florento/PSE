//
//  TestNoteName.cpp
//  testpse
//
//  Created by Florent Jacquemard on 31/03/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"


TEST(NoteName, toint)
{
    EXPECT_EQ(toint(pse::NoteName::C), 0);
    EXPECT_EQ(toint(pse::NoteName::D), 1);
    EXPECT_EQ(toint(pse::NoteName::E), 2);
    EXPECT_EQ(toint(pse::NoteName::F), 3);
    EXPECT_EQ(toint(pse::NoteName::G), 4);
    EXPECT_EQ(toint(pse::NoteName::A), 5);
    EXPECT_EQ(toint(pse::NoteName::B), 6);
    EXPECT_EQ(toint(pse::NoteName::Undef), 9);
}

TEST(NoteName, tochar)
{
    EXPECT_EQ(tochar(pse::NoteName::C), 'C');
    EXPECT_EQ(tochar(pse::NoteName::D), 'D');
    EXPECT_EQ(tochar(pse::NoteName::E), 'E');
    EXPECT_EQ(tochar(pse::NoteName::F), 'F');
    EXPECT_EQ(tochar(pse::NoteName::G), 'G');
    EXPECT_EQ(tochar(pse::NoteName::A), 'A');
    EXPECT_EQ(tochar(pse::NoteName::B), 'B');
    EXPECT_EQ(tochar(pse::NoteName::Undef), 'X');
}

TEST(NoteName, defined)
{
    EXPECT_TRUE(defined(pse::NoteName::C));
    EXPECT_TRUE(defined(pse::NoteName::D));
    EXPECT_TRUE(defined(pse::NoteName::E));
    EXPECT_TRUE(defined(pse::NoteName::F));
    EXPECT_TRUE(defined(pse::NoteName::G));
    EXPECT_TRUE(defined(pse::NoteName::A));
    EXPECT_TRUE(defined(pse::NoteName::B));
    EXPECT_FALSE(defined(pse::NoteName::Undef));
}

TEST(NoteName, constructorInt)
{
    EXPECT_EQ(pse::NoteName(0), pse::NoteName::C);
    EXPECT_EQ(pse::NoteName(1), pse::NoteName::D);
    EXPECT_EQ(pse::NoteName(2), pse::NoteName::E);
    EXPECT_EQ(pse::NoteName(3), pse::NoteName::F);
    EXPECT_EQ(pse::NoteName(4), pse::NoteName::G);
    EXPECT_EQ(pse::NoteName(5), pse::NoteName::A);
    EXPECT_EQ(pse::NoteName(6), pse::NoteName::B);
    EXPECT_EQ(pse::NoteName(9), pse::NoteName::Undef);
}

TEST(NoteName, constructorChar)
{
    EXPECT_EQ(pse::NoteName('C'), pse::NoteName::C);
    EXPECT_EQ(pse::NoteName('D'), pse::NoteName::D);
    EXPECT_EQ(pse::NoteName('E'), pse::NoteName::E);
    EXPECT_EQ(pse::NoteName('F'), pse::NoteName::F);
    EXPECT_EQ(pse::NoteName('G'), pse::NoteName::G);
    EXPECT_EQ(pse::NoteName('A'), pse::NoteName::A);
    EXPECT_EQ(pse::NoteName('B'), pse::NoteName::B);
    EXPECT_EQ(pse::NoteName('X'), pse::NoteName::Undef);
}

TEST(NoteName, diatonicStepUp)
{
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::C, pse::NoteName::C));
     EXPECT_TRUE(diatonicStepUp(pse::NoteName::C, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::C, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::C, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::C, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::C, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::C, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepUp(pse::NoteName::D, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::D, pse::NoteName::D));
     EXPECT_TRUE(diatonicStepUp(pse::NoteName::D, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::D, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::D, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::D, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::D, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepUp(pse::NoteName::E, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::E, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::E, pse::NoteName::E));
     EXPECT_TRUE(diatonicStepUp(pse::NoteName::E, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::E, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::E, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::E, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepUp(pse::NoteName::F, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::F, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::F, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::F, pse::NoteName::F));
     EXPECT_TRUE(diatonicStepUp(pse::NoteName::F, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::F, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::F, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepUp(pse::NoteName::G, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::G, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::G, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::G, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::G, pse::NoteName::G));
     EXPECT_TRUE(diatonicStepUp(pse::NoteName::G, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::G, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepUp(pse::NoteName::A, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::A, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::A, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::A, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::A, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::A, pse::NoteName::A));
    EXPECT_TRUE(diatonicStepUp(pse::NoteName::A, pse::NoteName::B));

     EXPECT_TRUE(diatonicStepUp(pse::NoteName::B, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::B, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::B, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::B, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::B, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::B, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepUp(pse::NoteName::B, pse::NoteName::B));

    //    WARNING
    for (int i = 0; i < 7; ++i)
    {
        enum pse::NoteName nn = pse::NoteName(i);
        EXPECT_FALSE(pse::diatonicStepUp(nn, pse::NoteName::Undef));
        //EXPECT_FALSE(pse::diatonicStepUp(pse::NoteName::Undef, nn));
    }
//    EXPECT_FALSE(pse::diatonicStepUp(pse::NoteName::Undef, pse::NoteName::Undef));
}


TEST(NoteName, diatonicStepDown)
{
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::C, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::C, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::C, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::C, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::C, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::C, pse::NoteName::A));
     EXPECT_TRUE(diatonicStepDown(pse::NoteName::C, pse::NoteName::B));

     EXPECT_TRUE(diatonicStepDown(pse::NoteName::D, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::D, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::D, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::D, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::D, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::D, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::D, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepDown(pse::NoteName::E, pse::NoteName::C));
     EXPECT_TRUE(diatonicStepDown(pse::NoteName::E, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::E, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::E, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::E, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::E, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::E, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepDown(pse::NoteName::F, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::F, pse::NoteName::D));
     EXPECT_TRUE(diatonicStepDown(pse::NoteName::F, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::F, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::F, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::F, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::F, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepDown(pse::NoteName::G, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::G, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::G, pse::NoteName::E));
     EXPECT_TRUE(diatonicStepDown(pse::NoteName::G, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::G, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::G, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::G, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepDown(pse::NoteName::A, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::A, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::A, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::A, pse::NoteName::F));
     EXPECT_TRUE(diatonicStepDown(pse::NoteName::A, pse::NoteName::G));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::A, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::A, pse::NoteName::B));

    EXPECT_FALSE(diatonicStepDown(pse::NoteName::B, pse::NoteName::C));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::B, pse::NoteName::D));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::B, pse::NoteName::E));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::B, pse::NoteName::F));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::B, pse::NoteName::G));
     EXPECT_TRUE(diatonicStepDown(pse::NoteName::B, pse::NoteName::A));
    EXPECT_FALSE(diatonicStepDown(pse::NoteName::B, pse::NoteName::B));

//    WARNING
    for (int i = 0; i < 7; ++i)
    {
        enum pse::NoteName nn = pse::NoteName(i);
        EXPECT_FALSE(pse::diatonicStepDown(nn, pse::NoteName::Undef));
//        EXPECT_FALSE(pse::diatonicStepDown(pse::NoteName::Undef, nn));
    }
//    EXPECT_FALSE(pse::diatonicStepDown(pse::NoteName::Undef, pse::NoteName::Undef));
}

TEST(NoteName, plus)
{
//    for (int i = 0; i < 7; ++i)
//    {
//        enum pse::NoteName n0 = pse::NoteName(i);
//        for (int d = -30; d < 31; ++d)
//        {
//            int r = i+d % 7;
//            if (r < 0) r += 7;
//            assert(0 <= r);
//            assert(r < 7);
//            enum pse::NoteName nd = pse::NoteName(r);
//            EXPECT_EQ(n0+d, nd);
//        }
//    }

    EXPECT_EQ(pse::NoteName::C+0, pse::NoteName::C);
    EXPECT_EQ(pse::NoteName::C+1, pse::NoteName::D);
    EXPECT_EQ(pse::NoteName::C+4, pse::NoteName::G);
    EXPECT_EQ(pse::NoteName::C+5, pse::NoteName::A);
    EXPECT_EQ(pse::NoteName::C+6, pse::NoteName::B);
    EXPECT_EQ(pse::NoteName::C+7, pse::NoteName::C);
    EXPECT_EQ(pse::NoteName::C+9, pse::NoteName::E);
    EXPECT_EQ(pse::NoteName::C+(-1), pse::NoteName::B);
    EXPECT_EQ(pse::NoteName::C+(-2), pse::NoteName::A);
    EXPECT_EQ(pse::NoteName::C+(-6), pse::NoteName::D);
    EXPECT_EQ(pse::NoteName::C+(-7), pse::NoteName::C);
    EXPECT_EQ(pse::NoteName::C+(-11), pse::NoteName::F);

    EXPECT_EQ(pse::NoteName::B+0, pse::NoteName::B);
    EXPECT_EQ(pse::NoteName::B+1, pse::NoteName::C);
    EXPECT_EQ(pse::NoteName::B+3, pse::NoteName::E);
    EXPECT_EQ(pse::NoteName::B+13, pse::NoteName::C+12);
    
    EXPECT_EQ(pse::NoteName::Undef+0, pse::NoteName::Undef);
    EXPECT_EQ(pse::NoteName::Undef+1, pse::NoteName::Undef);
    EXPECT_EQ(pse::NoteName::Undef+5, pse::NoteName::Undef);
    EXPECT_EQ(pse::NoteName::Undef+7, pse::NoteName::Undef);
    EXPECT_EQ(pse::NoteName::Undef+11, pse::NoteName::Undef);
    EXPECT_EQ(pse::NoteName::Undef+(-1), pse::NoteName::Undef);
    EXPECT_EQ(pse::NoteName::Undef+(-11), pse::NoteName::Undef);
}

