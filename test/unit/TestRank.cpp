//
//  TestRank.cpp
//  testpse
//
//  Created by Florent Jacquemard on 01/07/2023.
//

#include "gtest/gtest.h"

#include <vector>

#include "utils.hpp"


TEST(Rank, t0)
{
    std::vector<unsigned int> v;

    std::vector<size_t> r;
    pse::util::ranks<unsigned int>(v,
                        [](unsigned int a, unsigned int b) { return (a == b); },
                        [](unsigned int a, unsigned int b) { return (a < b); },
                                   r);

    ASSERT_EQ(r.size(), 0);
}

TEST(Rank, t1)
{
    std::vector<unsigned int> v;
    v.push_back(0);

    std::vector<size_t> r;
    pse::util::ranks<unsigned int>(v,
                        [](unsigned int a, unsigned int b) { return (a == b); },
                        [](unsigned int a, unsigned int b) { return (a < b); },
                                   r);

    ASSERT_EQ(r.size(), 1);
    EXPECT_EQ(r[0], 0);
}

TEST(Rank, t2)
{
    std::vector<unsigned int> v;
    v.push_back(100);
    v.push_back(101);

    std::vector<size_t> r;
    pse::util::ranks<unsigned int>(v,
                        [](unsigned int a, unsigned int b) { return (a == b); },
                        [](unsigned int a, unsigned int b) { return (a < b); },
                                   r);

    ASSERT_EQ(r.size(), 2);
    EXPECT_EQ(r[0], 0);
    EXPECT_EQ(r[1], 1);
}

TEST(Rank, t2a)
{
    std::vector<unsigned int> v;
    v.push_back(101);
    v.push_back(100);

    std::vector<size_t> r;
    pse::util::ranks<unsigned int>(v,
                        [](unsigned int a, unsigned int b) { return (a == b); },
                        [](unsigned int a, unsigned int b) { return (a < b); },
                                   r);

    ASSERT_EQ(r.size(), 2);
    EXPECT_EQ(r[0], 1);
    EXPECT_EQ(r[1], 0);
}

TEST(Rank, t2b)
{
    std::vector<unsigned int> v;
    v.push_back(100);
    v.push_back(100);

    std::vector<size_t> r;
    pse::util::ranks<unsigned int>(v,
                        [](unsigned int a, unsigned int b) { return (a == b); },
                        [](unsigned int a, unsigned int b) { return (a < b); },
                                   r);

    ASSERT_EQ(r.size(), 2);
    EXPECT_EQ(r[0], 0);
    EXPECT_EQ(r[1], 0);
}

TEST(Rank, t5)
{
    std::vector<unsigned int> v;
    v.push_back(102); // 3
    v.push_back(100); // 0
    v.push_back(109); // 4
    v.push_back(100); // 0
    v.push_back(101); // 2

    std::vector<size_t> r;
    pse::util::ranks<unsigned int>(v,
                        [](unsigned int a, unsigned int b) { return (a == b); },
                        [](unsigned int a, unsigned int b) { return (a < b); },
                                   r);

    ASSERT_EQ(r.size(), 5);
    EXPECT_EQ(r[0], 3);
    EXPECT_EQ(r[1], 0);
    EXPECT_EQ(r[2], 4);
    EXPECT_EQ(r[3], 0);
    EXPECT_EQ(r[4], 2);
}

TEST(Rank, t6)
{
    std::vector<unsigned int> v;
    v.push_back(101); // 1
    v.push_back(100); // 0
    v.push_back(107); // 5
    v.push_back(101); // 1
    v.push_back(106); // 4
    v.push_back(101); // 1

    std::vector<size_t> r;
    pse::util::ranks<unsigned int>(v,
                        [](unsigned int a, unsigned int b) { return (a == b); },
                        [](unsigned int a, unsigned int b) { return (a < b); },
                                   r);

    ASSERT_EQ(r.size(), 6);
    EXPECT_EQ(r[0], 1);
    EXPECT_EQ(r[1], 0);
    EXPECT_EQ(r[2], 5);
    EXPECT_EQ(r[3], 1);
    EXPECT_EQ(r[4], 4);
    EXPECT_EQ(r[5], 1);
}
