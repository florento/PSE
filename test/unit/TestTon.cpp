//
//  TestTon.cpp
//  testpse
//
//  Created by Florent Jacquemard on 01/06/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"



TEST(Ton, Undef)
{
    pse::Ton t_U;
    pse::Ton t_CM(0, pse::ModeName::Major);
    EXPECT_TRUE(t_U.undef());
    EXPECT_FALSE(t_CM.undef());
}


TEST(Ton, C_maj)
{
    pse::Ton t(0, pse::ModeName::Major);

    EXPECT_FALSE(t.undef());
    EXPECT_EQ(t.getPitchClass(), 0);
    EXPECT_EQ(t.getName(), pse::NoteName::C);
    EXPECT_EQ(t.getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(t.getMode(), pse::ModeName::Major);
    
    EXPECT_EQ(t.accidDia(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::E), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::A), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::B), pse::Accid::Natural);
}


TEST(Ton, C_min)
{
    pse::Ton t(-3, pse::ModeName::Minor);
   
    EXPECT_FALSE(t.undef());
    EXPECT_EQ(t.getPitchClass(), 0);
    EXPECT_EQ(t.getName(), pse::NoteName::C);
    EXPECT_EQ(t.getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(t.getMode(), pse::ModeName::Minor);
    
    EXPECT_EQ(t.accidKey(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(t.accidKey(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(t.accidKey(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(t.accidKey(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(t.accidKey(pse::NoteName::A), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::B), pse::Accid::Flat);
    
    EXPECT_EQ(t.accidDia(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::A), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::B), pse::Accid::Natural);
}


TEST(Ton, D_maj)
{
    pse::Ton t(2, pse::ModeName::Major);

    EXPECT_FALSE(t.undef());
    EXPECT_EQ(t.getPitchClass(), 2);
    EXPECT_EQ(t.getName(), pse::NoteName::D);
    EXPECT_EQ(t.getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(t.getMode(), pse::ModeName::Major);
    
    EXPECT_EQ(t.accidDia(pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::E), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::A), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::B), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::C), pse::Accid::Sharp);
}


TEST(Ton, B_maj)
{
    pse::Ton t(5, pse::ModeName::Major);

    EXPECT_FALSE(t.undef());
    EXPECT_EQ(t.getPitchClass(), 11);
    EXPECT_EQ(t.getName(), pse::NoteName::B);
    EXPECT_EQ(t.getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(t.getMode(), pse::ModeName::Major);
    
    EXPECT_EQ(t.accidDia(pse::NoteName::B), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::C), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::D), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::E), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::G), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::A), pse::Accid::Sharp);
}


TEST(Ton, Cb_maj)
{
    pse::Ton t(-7, pse::ModeName::Major);

    EXPECT_FALSE(t.undef());
    EXPECT_EQ(t.getPitchClass(), 11);
    EXPECT_EQ(t.getName(), pse::NoteName::C);
    EXPECT_EQ(t.getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(t.getMode(), pse::ModeName::Major);

    EXPECT_EQ(t.accidKey(pse::NoteName::C), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::D), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::F), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::G), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::A), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::B), pse::Accid::Flat);

    EXPECT_EQ(t.accidDia(pse::NoteName::C), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::D), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::F), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::G), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::A), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::B), pse::Accid::Flat);
}

TEST(Ton, Bb_min)
{
    pse::Ton t(-5, pse::ModeName::Minor);
    
    EXPECT_FALSE(t.undef());
    EXPECT_EQ(t.getPitchClass(), 10);
    EXPECT_EQ(t.getName(), pse::NoteName::B);
    EXPECT_EQ(t.getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(t.getMode(), pse::ModeName::Minor);
    
    EXPECT_EQ(t.accidKey(pse::NoteName::B), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(t.accidKey(pse::NoteName::D), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(t.accidKey(pse::NoteName::G), pse::Accid::Flat);
    EXPECT_EQ(t.accidKey(pse::NoteName::A), pse::Accid::Flat);
    
    EXPECT_EQ(t.accidDia(pse::NoteName::B), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::D), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(t.accidDia(pse::NoteName::G), pse::Accid::Flat);
    EXPECT_EQ(t.accidDia(pse::NoteName::A), pse::Accid::Natural);
}

TEST(Ton, As_min)
{
    pse::Ton t(7, pse::ModeName::Minor);

    EXPECT_FALSE(t.undef());
    EXPECT_EQ(t.getPitchClass(), 10);
    EXPECT_EQ(t.getName(), pse::NoteName::A);
    EXPECT_EQ(t.getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(t.getMode(), pse::ModeName::Minor);
    
    EXPECT_EQ(t.accidKey(pse::NoteName::A), pse::Accid::Sharp);
    EXPECT_EQ(t.accidKey(pse::NoteName::B), pse::Accid::Sharp);
    EXPECT_EQ(t.accidKey(pse::NoteName::C), pse::Accid::Sharp);
    EXPECT_EQ(t.accidKey(pse::NoteName::D), pse::Accid::Sharp);
    EXPECT_EQ(t.accidKey(pse::NoteName::E), pse::Accid::Sharp);
    EXPECT_EQ(t.accidKey(pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(t.accidKey(pse::NoteName::G), pse::Accid::Sharp);

    EXPECT_EQ(t.accidDia(pse::NoteName::A), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::B), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::C), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::D), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::E), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(t.accidDia(pse::NoteName::G), pse::Accid::DoubleSharp);
}
