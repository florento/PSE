//
//  TestCostADlex.cpp
//  testpse
//
//  Created by Florent Jacquemard on 29/06/2023.
//

#include "gtest/gtest.h"

#include "Ton.hpp"
#include "CostADlex.hpp"

// ASAP Bach BWV 858 Prelude bar 26
// An D(b) G(b) C(b) F(n) Ab D(b) G(b) An Dn Ab C(b)
// accid=3 dist=2 color=0 cflat=0
TEST(TestCostADlex, BWV858_26)
{
    // Gb maj : Cb Db Eb Fn Gb Ab Bb
    const pse::Ton gton(-6, pse::ModeName::Major);
  
    // F# min : C# Dn E#* F# G# An Bn
    const pse::Ton lton(3, pse::ModeName::Minor);
    
    pse::CostADlex cost; // zero
    std::shared_ptr<pse::Cost> zero = cost.shared_zero();
    EXPECT_NE(zero, nullptr);
    EXPECT_EQ(cost, *zero);
    EXPECT_EQ(cost.get_accid(), 0);
    EXPECT_EQ(cost.get_dist(),  0);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::A, pse::Accid::Natural, true, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 1);
    EXPECT_EQ(cost.get_dist(),  0);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::D, pse::Accid::Flat, false, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 1);
    EXPECT_EQ(cost.get_dist(),  1);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::G, pse::Accid::Flat, false, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 1);
    EXPECT_EQ(cost.get_dist(),  2);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::C, pse::Accid::Flat, false, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 1);
    EXPECT_EQ(cost.get_dist(),  3);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::F, pse::Accid::Natural, false, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 1);
    EXPECT_EQ(cost.get_dist(),  4);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::A, pse::Accid::Flat, true, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 2);
    EXPECT_EQ(cost.get_dist(),  5);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);
    
    cost.update(pse::NoteName::D, pse::Accid::Flat, false, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 2);
    EXPECT_EQ(cost.get_dist(),  6);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::G, pse::Accid::Flat, false, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 2);
    EXPECT_EQ(cost.get_dist(),  7);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::A, pse::Accid::Natural, true, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 3);
    EXPECT_EQ(cost.get_dist(),  7);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::D, pse::Accid::Natural, true, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 4);
    EXPECT_EQ(cost.get_dist(),  7);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);
    
    cost.update(pse::NoteName::A, pse::Accid::Flat, true, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 5);
    EXPECT_EQ(cost.get_dist(),  8);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

    cost.update(pse::NoteName::C, pse::Accid::Flat, false, // print
                gton, lton);
    EXPECT_EQ(cost.get_accid(), 5);
    EXPECT_EQ(cost.get_dist(),  9);
    EXPECT_EQ(cost.get_color(), 0);
    EXPECT_EQ(cost.get_cflat(), 0);

}

