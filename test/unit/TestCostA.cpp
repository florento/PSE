//
//  TestCostA.cpp
//  testpse
//
//  Created by Florent Jacquemard on 18/04/2024.
//

#include "gtest/gtest.h"

#include "Ton.hpp"
#include "CostA.hpp"


TEST(CostA, zero)
{
    pse::CostA c0; // no discount in updates
    
    std::shared_ptr<pse::Cost> null = c0.shared_zero();
    EXPECT_NE(null, nullptr);
    std::shared_ptr<pse::Cost> clone = c0.shared_clone();
    EXPECT_NE(clone, nullptr);
    std::unique_ptr<pse::Cost> clonu = c0.unique_clone();
    EXPECT_NE(clonu, nullptr);
    
    EXPECT_TRUE(*clone == *null);
    EXPECT_FALSE(*clone < *null);

    EXPECT_EQ(c0.get_accid(), 0);
}

TEST(CostA, update1)
{
    pse::CostA c0; // no discount in updates
    std::shared_ptr<pse::Cost> null = c0.shared_zero();
    EXPECT_NE(null, nullptr);
    std::shared_ptr<pse::Cost> clone = c0.shared_clone();
    EXPECT_NE(clone, nullptr);
    std::unique_ptr<pse::Cost> clonu = c0.unique_clone();
    EXPECT_NE(clonu, nullptr);

    pse::CostA c1(c0);    // copy
    // print, gton and lton are ignored
    c1.update(pse::NoteName::F, pse::Accid::Sharp, true,
              pse::Ton(0, pse::ModeName::Major), pse::Ton());

    ASSERT_EQ(c1.get_accid(), 1); // cost c1 = 1 accid
    ASSERT_FALSE(c0 == c1);
    ASSERT_TRUE(c0 < c1);
    ASSERT_TRUE(c1 >= c0);

    std::shared_ptr<pse::Cost> clone1 = c1.shared_clone();
    EXPECT_NE(clone1, nullptr);
    EXPECT_TRUE(*null < *clone1);
}

TEST(CostA, update2)
{
    pse::CostA c0; // no discount in updates

    pse::CostA c1(c0);    // copy
    // print, gton and lton are ignored
    c1.update(pse::NoteName::F, pse::Accid::Sharp, true,
              pse::Ton(0, pse::ModeName::Major), pse::Ton());

    pse::CostA c2a(c0);    // copy
    c2a.update(pse::NoteName::B, pse::Accid::DoubleFlat, true,
               pse::Ton(0, pse::ModeName::Major), pse::Ton());

    pse::CostA c2b(c1);    // copy
    c2b.update(pse::NoteName::B, pse::Accid::Flat, true,
               pse::Ton(0, pse::ModeName::Major), pse::Ton());

    pse::CostA c2c(c1);    // copy
    c2c += c1;
    
    ASSERT_EQ(c1.get_accid(), 1);
    ASSERT_EQ(c2a.get_accid(), 2);
    ASSERT_EQ(c2b.get_accid(), 2);
    ASSERT_EQ(c2c.get_accid(), 2);

    ASSERT_FALSE(c1 == c2a);
    ASSERT_TRUE(c0 < c1);
    ASSERT_TRUE(c1 < c2a);
    ASSERT_TRUE(c1 < c2b);
    ASSERT_TRUE(c1 < c2c);
    ASSERT_TRUE(c2a == c2b);
    ASSERT_TRUE(c2b == c2c);
    ASSERT_TRUE(c2b >= c1);
}

