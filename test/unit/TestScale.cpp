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

TEST(Scale, D_maj)
{
    pse::Scale s(pse::ModeName::Major, 2, pse::NoteName::D);
    pse::Ton t(2, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(6));
}

TEST(Scale, Eflat_maj)
{
    pse::Scale s(pse::ModeName::Major, 3, pse::NoteName::E);
    pse::Ton t(-3, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(6));
}

TEST(Scale, E_maj)
{
    pse::Scale s(pse::ModeName::Major, 4, pse::NoteName::E);
    pse::Ton t(4, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(6));
}

TEST(Scale, F_maj)
{
    pse::Scale s(pse::ModeName::Major, 5, pse::NoteName::F);
    pse::Ton t(-1, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(6));
}

TEST(Scale, Fsharp_maj)
{
    pse::Scale s(pse::ModeName::Major, 6, pse::NoteName::F);
    pse::Ton t(6, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(6));
}

TEST(Scale, Gflat_maj)
{
    pse::Scale s(pse::ModeName::Major, 6, pse::NoteName::G);
    pse::Ton t(-6, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(6));
}

TEST(Scale, G_maj)
{
    pse::Scale s(pse::ModeName::Major, 7, pse::NoteName::G);
    pse::Ton t(1, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(6));
}

TEST(Scale, Aflat_maj)
{
    pse::Scale s(pse::ModeName::Major, 8, pse::NoteName::A);
    pse::Ton t(-4, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(6));
}

TEST(Scale, A_maj)
{
    pse::Scale s(pse::ModeName::Major, 9, pse::NoteName::A);
    pse::Ton t(3, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(6));
}

TEST(Scale, Bflat_maj)
{
    pse::Scale s(pse::ModeName::Major, 10, pse::NoteName::B);
    pse::Ton t(-2, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(6));
}

TEST(Scale, B_maj)
{
    pse::Scale s(pse::ModeName::Major, 11, pse::NoteName::B);
    pse::Ton t(5, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(6));
}

TEST(Scale, Cflat_maj)
{
    pse::Scale s(pse::ModeName::Major, 11, pse::NoteName::C);
    pse::Ton t(-7, pse::ModeName::Major);

    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(6));
}






TEST(Scale, C_min)
{
    // tonic: pitch class 0 = C
    pse::Scale s(pse::ModeName::Minor, 0, pse::NoteName::C);

    // key sig 0. mode maj = see table Ton::KEYS
    pse::Ton t(-3, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(6));
}

TEST(Scale, Csharp_min)
{
    pse::Scale s(pse::ModeName::Minor, 1, pse::NoteName::C);
    pse::Ton t(4, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(6));
}

TEST(Scale, D_min)
{
    pse::Scale s(pse::ModeName::Minor, 2, pse::NoteName::D);
    pse::Ton t(-1, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(6));
}

TEST(Scale, Dsharp_min)
{
    pse::Scale s(pse::ModeName::Minor, 3, pse::NoteName::D);
    pse::Ton t(6, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(6));
}

TEST(Scale, Eflat_min)
{
    pse::Scale s(pse::ModeName::Minor, 3, pse::NoteName::E);
    pse::Ton t(-6, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(6));
}

TEST(Scale, E_min)
{
    pse::Scale s(pse::ModeName::Minor, 4, pse::NoteName::E);
    pse::Ton t(1, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(0)); // degree 0
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(6));
}

TEST(Scale, F_min)
{
    pse::Scale s(pse::ModeName::Minor, 5, pse::NoteName::F);
    pse::Ton t(-4, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(6));
}

TEST(Scale, Fsharp_min)
{
    pse::Scale s(pse::ModeName::Minor, 6, pse::NoteName::F);
    pse::Ton t(3, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(6));
}

TEST(Scale, G_min)
{
    pse::Scale s(pse::ModeName::Minor, 7, pse::NoteName::G);
    pse::Ton t(-2, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(6));
}

TEST(Scale, Gsharp_min)
{
    pse::Scale s(pse::ModeName::Minor, 8, pse::NoteName::G);
    pse::Ton t(5, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(6));
}

TEST(Scale, Aflat_min)
{
    pse::Scale s(pse::ModeName::Minor, 8, pse::NoteName::A);
    pse::Ton t(-7, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(6));
}

TEST(Scale, A_min)
{
    pse::Scale s(pse::ModeName::Minor, 9, pse::NoteName::A);
    pse::Ton t(0, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(6));
}

TEST(Scale, Asharp_min)
{
    pse::Scale s(pse::ModeName::Minor, 10, pse::NoteName::A);
    pse::Ton t(7, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(6));
}

TEST(Scale, Bflat_min)
{
    pse::Scale s(pse::ModeName::Minor, 10, pse::NoteName::B);
    pse::Ton t(-5, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(6));
}

TEST(Scale, B_min)
{
    pse::Scale s(pse::ModeName::Minor, 11, pse::NoteName::B);
    pse::Ton t(2, pse::ModeName::Minor);

    EXPECT_EQ(t.accidDia(pse::NoteName::B), s.accid(0));
    EXPECT_EQ(t.accidDia(pse::NoteName::C), s.accid(1));
    EXPECT_EQ(t.accidDia(pse::NoteName::D), s.accid(2));
    EXPECT_EQ(t.accidDia(pse::NoteName::E), s.accid(3));
    EXPECT_EQ(t.accidDia(pse::NoteName::F), s.accid(4));
    EXPECT_EQ(t.accidDia(pse::NoteName::G), s.accid(5));
    EXPECT_EQ(t.accidDia(pse::NoteName::A), s.accid(6));
}



// harmonic chromatic scale
// see https://www.dolmetsch.com/musictheory11.htm
// - includes all the notes in the major scale
// - includes all the notes in the harmonic minor scale
// - includes all the notes in the melodic minor scale
// - plus flattened second degree
// - plus sharpened fourth degree.
//
// This leads to
// - a single fifth
// - single tonic key-note,
// - single octave key-note
// - pairs of every other degree.
// This means that the notation of a harmonic chromatic scale varies
// according to the key signature.

// explicit accidentals
TEST(Scale, C_chroma0)
{
    pse::Scale s(pse::ModeName::Chromatic, 0, pse::NoteName::C);

    EXPECT_EQ(s.accid(0),  pse::Accid::Natural); // C
    EXPECT_EQ(s.accid(1),  pse::Accid::Flat);    // Db
    EXPECT_EQ(s.accid(2),  pse::Accid::Natural); // D
    EXPECT_EQ(s.accid(3),  pse::Accid::Flat);    // Eb
    EXPECT_EQ(s.accid(4),  pse::Accid::Natural); // E
    EXPECT_EQ(s.accid(5),  pse::Accid::Natural); // F
    EXPECT_EQ(s.accid(6),  pse::Accid::Sharp);   // F#
    EXPECT_EQ(s.accid(7),  pse::Accid::Natural); // G
    EXPECT_EQ(s.accid(8),  pse::Accid::Flat);    // Ab
    EXPECT_EQ(s.accid(9),  pse::Accid::Natural); // A
    EXPECT_EQ(s.accid(10), pse::Accid::Flat);    // Bb
    EXPECT_EQ(s.accid(11), pse::Accid::Natural); // B
}

// - includes all the notes in the major scale
// - includes all the notes in the harmonic minor scale
// - includes all the notes in the melodic natural minor scale
// - plus flattened second degree
// - plus sharpened fourth degree.
TEST(Scale, C_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 0, pse::NoteName::C);
    pse::Ton maj(0, pse::ModeName::Major);
    pse::Ton har(-3, pse::ModeName::Minor);
    pse::Ton nat(-3, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, Csharp_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 1, pse::NoteName::C);
    pse::Ton maj(7, pse::ModeName::Major);
    pse::Ton har(4, pse::ModeName::Minor);
    pse::Ton nat(4, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, D_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 2, pse::NoteName::D);
    pse::Ton maj(2, pse::ModeName::Major);
    pse::Ton har(-1, pse::ModeName::Minor);
    pse::Ton nat(-1, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, Eflat_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 3, pse::NoteName::E);
    pse::Ton maj(-3, pse::ModeName::Major);
    pse::Ton har(-6, pse::ModeName::Minor);
    pse::Ton nat(-6, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, E_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 4, pse::NoteName::E);
    pse::Ton maj(4, pse::ModeName::Major);
    pse::Ton har(1, pse::ModeName::Minor);
    pse::Ton nat(1, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, F_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 5, pse::NoteName::F);
    pse::Ton maj(-1, pse::ModeName::Major);
    pse::Ton har(-4, pse::ModeName::Minor);
    pse::Ton nat(-4, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, Fsharp_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 6, pse::NoteName::F);
    pse::Ton maj(6, pse::ModeName::Major);
    pse::Ton har(3, pse::ModeName::Minor);
    pse::Ton nat(3, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, G_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 7, pse::NoteName::G);
    pse::Ton maj(1, pse::ModeName::Major);
    pse::Ton har(-2, pse::ModeName::Minor);
    pse::Ton nat(-2, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, Aflat_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 8, pse::NoteName::A);
    pse::Ton maj(-4, pse::ModeName::Major);
    pse::Ton har(-7, pse::ModeName::Minor);
    pse::Ton nat(-7, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, A_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 9, pse::NoteName::A);
    pse::Ton maj(3, pse::ModeName::Major);
    pse::Ton har(0, pse::ModeName::Minor);
    pse::Ton nat(0, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, Bflat_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 10, pse::NoteName::B);
    pse::Ton maj(-2, pse::ModeName::Major);
    pse::Ton har(-5, pse::ModeName::Minor);
    pse::Ton nat(-5, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}

TEST(Scale, B_chroma)
{
    pse::Scale s(pse::ModeName::Chromatic, 11, pse::NoteName::B);
    pse::Ton maj(5, pse::ModeName::Major);
    pse::Ton har(2, pse::ModeName::Minor);
    pse::Ton nat(2, pse::ModeName::MinorNat);
    
    EXPECT_EQ(s.name(0),  maj.name(0));     // degree I  (same for min)
    EXPECT_EQ(s.accid(0),  maj.accid(0));
    EXPECT_EQ(s.name(1),  maj.name(1));     // flattened degree II
    EXPECT_EQ(s.accid(1),  --(maj.accid(1)));
    EXPECT_EQ(s.name(2),  maj.name(1));     // degree II (same for min)
    EXPECT_EQ(s.accid(2),  maj.accid(1));
    EXPECT_EQ(s.name(3),  har.name(2));     // degree III
    EXPECT_EQ(s.accid(3),  har.accid(2));
    EXPECT_EQ(s.name(4),  maj.name(2));     // degree III
    EXPECT_EQ(s.accid(4),  maj.accid(2));
    EXPECT_EQ(s.name(5),  maj.name(3));     // degree IV (same for min)
    EXPECT_EQ(s.accid(5),  maj.accid(3));
    EXPECT_EQ(s.name(6),  maj.name(3));     // sharpened degree IV
    EXPECT_EQ(s.accid(6),  ++(maj.accid(3)));
    EXPECT_EQ(s.name(7),  maj.name(4));     // degree V (same for min)
    EXPECT_EQ(s.accid(7),  maj.accid(4));
    EXPECT_EQ(s.name(8),  har.name(5));     // degree VI
    EXPECT_EQ(s.accid(8),  har.accid(5));
    EXPECT_EQ(s.name(9),  maj.name(5));     // degree VI
    EXPECT_EQ(s.accid(9),  maj.accid(5));
    EXPECT_EQ(s.name(10),  nat.name(6));    // degree VII
    EXPECT_EQ(s.accid(10),  nat.accid(6));
    EXPECT_EQ(s.name(11),  har.name(6));    // degree VII (same for maj)
    EXPECT_EQ(s.accid(11),  har.accid(6));
}
