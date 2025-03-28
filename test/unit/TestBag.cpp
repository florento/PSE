//
//  TestBag.cpp
//  testpse
//
//  Created by Florent Jacquemard on 18/04/2024.
//

#include "gtest/gtest.h"

#include "Ton.hpp"
#include "PSRawEnum.hpp"
#include "CostA.hpp"
#include "PSBag.hpp"

TEST(PSBag, Eb_maj0)
{
    pse::Ton t(-3, pse::ModeName::Major);
    pse::PSRawEnum e(0, 4);
    pse::CostA c0; // no discount in updates
    
    e.add(55, 0); // G3
    e.add(56, 0); // Ab3
    e.add(58, 0); // Bb3
    e.add(55, 0); // G3
      
    // gton=t, lton-UNDEF, tonal flag
    pse::PSB b(pse::Algo::PSE, c0, e, true, t, pse::Ton());
    
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.cost(), c0);
}

TEST(PSBag, Eb_maj1)
{
    
    pse::Ton t(-3, pse::ModeName::Major);
    pse::PSRawEnum e(0, 4);
    pse::CostA c0; // no discount in updates
    
    pse::CostA c1(c0);    // copy
    // print, gton=t, lton-UNDEF
    c1.update(pse::NoteName::F, pse::Accid::Sharp, true, t, pse::Ton());
    ASSERT_EQ(c1.get_accid(), 1); // cost c1 = 1 accid

    
    e.add(55, 0); // G3
    e.add(56, 0); // Ab3
    e.add(58, 0); // Bb3
    e.add(54, 0); // G3
      
    // gton=t, lton-UNDEF, tonal flag
    pse::PSB b(pse::Algo::PSE, c0, e, true, t, pse::Ton());
    
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.cost(), c1);
}

TEST(PSBag, Eb_maj2)
{
    
    pse::Ton t(-3, pse::ModeName::Major);
    pse::PSRawEnum e(0, 5);
    pse::CostA c0; // no discount in updates
    
    pse::CostA c1(c0);    // copy
    // print, gton=t, lton-UNDEF
    c1.update(pse::NoteName::F, pse::Accid::Sharp, true, t, pse::Ton());
    ASSERT_EQ(c1.get_accid(), 1); // cost c1 = 1 accid
    
    e.add(50, 0); // D3
    e.add(62, 0); // D4
    e.add(60, 0); // C4
    e.add(58, 0); // Bb3
    e.add(57, 0); // A3
      
    // gton=t, lton-UNDEF, tonal flag
    pse::PSB b(pse::Algo::PSE, c0, e, true, t, pse::Ton());
    
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.cost(), c1);
}

TEST(PSBag, Eb_maj3)
{
    
    pse::Ton t(-3, pse::ModeName::Major);
    pse::PSRawEnum e(0, 5);
    pse::CostA c0; // no discount in updates
    
    e.add(55, 0); // G3
    e.add(51, 0); // Eb3
    e.add(50, 0); // D3
    e.add(48, 0); // C3
    e.add(46, 0); // Bb2
      
    // gton=t, lton-UNDEF, tonal flag
    pse::PSB b(pse::Algo::PSE, c0, e, true, t, pse::Ton());
    
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.cost(), c0);
}

TEST(PSBag, Cmin3)
{
    
    pse::Ton t(-3, pse::ModeName::Minor); // harmonic
    pse::PSRawEnum e(0, 5);
    pse::CostA c0; // no discount in updates
    
    e.add(55, 0); // G3
    e.add(51, 0); // Eb3
    e.add(50, 0); // D3
    e.add(48, 0); // C3
    e.add(46, 0); // Bb2
      
    // gton=t, lton-UNDEF, tonal flag
    pse::PSB b(pse::Algo::PSE, c0, e, true, t, pse::Ton());
    
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.cost(), c0);
}

TEST(PSBag, Cmin3d)
{
    
    pse::Ton t(-3, pse::ModeName::Minor); // harmonic
    pse::PSRawEnum e(0, 5);
    pse::CostA c0; // no discount in updates
    pse::CostA c1(c0);    // copy
    // print, gton=t, lton-UNDEF
    c1.update(pse::NoteName::F, pse::Accid::Sharp, true, t, pse::Ton());
    ASSERT_EQ(c1.get_accid(), 1); // cost c1 = 1 accid
        
    e.add(55, 0); // G3
    e.add(51, 0); // Eb3
    e.add(50, 0); // D3
    e.add(48, 0); // C3
    e.add(46, 0); // Bb2
      
    // gton=t, lton-UNDEF, modal flag
    pse::PSB b(pse::Algo::PSE, c0, e, false, t, pse::Ton());
    
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.cost(), c1);
}


