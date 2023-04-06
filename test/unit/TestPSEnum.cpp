//
//  TestPSRawEnum.cpp
//  testpse
//
//  Created by Florent Jacquemard on 05/04/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Scale.hpp"
#include "Ton.hpp"
#include "PSEnum.hpp"
#include "PSRawEnum.hpp"


//class TestPSRawEnum : public ::testing::Test
//{
//protected:
//
//    void SetUp() override
//    {
//        _enum = pse::PSRawEnum(0, 12); // empty
//        _enum.add();
//    }
//
//    void TearDown() override
//    {
//        //ASSERT_NE(sc, nullptr);
//        // delete sc;
//    }
//
//    // Measure* currentMeasure() { return mes; }
//    pse::PSRawEnum _enum;
//};


TEST(PSEnum, empty)
{
    pse::PSRawEnum e0(0, 0); // empty
    EXPECT_TRUE(e0.empty());
    EXPECT_FALSE(e0.open());
    EXPECT_EQ(e0.first(), 0);
    EXPECT_EQ(e0.stop(), 0);
    EXPECT_EQ(e0.length(), 0);
    EXPECT_FALSE(e0.inside(0));
    EXPECT_FALSE(e0.inside(1));
    EXPECT_TRUE(e0.outside(0));
}

TEST(PSEnum, open)
{
    pse::PSRawEnum e(0);
    EXPECT_FALSE(e.empty());
    EXPECT_TRUE(e.open());
    EXPECT_EQ(e.first(), 0);
    EXPECT_TRUE(e.inside(0));
    EXPECT_TRUE(e.inside(1));
    EXPECT_TRUE(e.inside(1000));
}

TEST(PSEnum, base)
{
    pse::PSRawEnum e(0, 12);
    EXPECT_FALSE(e.empty());
    EXPECT_FALSE(e.open());
    EXPECT_EQ(e.first(), 0);
    EXPECT_EQ(e.stop(), 12);
    EXPECT_TRUE(e.inside(0));
    EXPECT_TRUE(e.inside(1));
    EXPECT_TRUE(e.inside(11));
    EXPECT_FALSE(e.inside(12));
    EXPECT_TRUE(e.outside(1000));
}

