//
//  TestRankWeber.cpp
//  testpse
//
//  Created by Florent Jacquemard on 01/07/2023.
//

#include "gtest/gtest.h"

#include "TonIndex.hpp"
#include "Weber.hpp"

 
TEST(RankWeber, ton1)
{
    pse::TonIndex index;
    index.add(0, pse::ModeName::Major); // C maj
    index.close();
    ASSERT_EQ(index.rankWeber(0, 0), 0);
}


TEST(RankWeber, ton3)
{
    pse::TonIndex index;
    index.add(0, pse::ModeName::Major); // C maj
    index.add(1, pse::ModeName::Major); // G maj
    index.add(2, pse::ModeName::Major); // D maj
    index.close();

    ASSERT_EQ(index.rankWeber(0, 0), 0);
    ASSERT_EQ(index.rankWeber(0, 1), 1);
    ASSERT_EQ(index.rankWeber(0, 2), 2);

    ASSERT_EQ(index.rankWeber(1, 0), 1);
    ASSERT_EQ(index.rankWeber(1, 1), 0);
    ASSERT_EQ(index.rankWeber(1, 2), 1);

    ASSERT_EQ(index.rankWeber(2, 0), 2);
    ASSERT_EQ(index.rankWeber(2, 1), 1);
    ASSERT_EQ(index.rankWeber(2, 2), 0);
}


TEST(RankWeber, ton30)
{
    pse::TonIndex index(30);
    for (size_t i = 0; i < 30; ++i)
    {
        const pse::Ton& toni = index.ton(i);
        for (size_t j = 0; j < 30; ++j)
        {
            unsigned int dij = index.distWeber(i, j);
            // const pse::Ton& tonj = index.ton(j);
            EXPECT_EQ(dij, toni.distWeber(index.ton(j)));
            size_t rij = index.rankWeber(i, j);
            
            for (size_t k = 0; k < j; ++k)
            {
                unsigned int dik = index.distWeber(i, k);
                // const pse::Ton& tonk = index.ton(k);
                EXPECT_EQ(dik, toni.distWeber(index.ton(k)));
                size_t rik = index.rankWeber(i, k);
                // assert((rij == rik) == (dij == dik));
                // assert((rij  < rik) == (dij < dik));
                // assert((rij  > rik) == (dij > dik));
                
                // INFO("i = {} j = {} dij = {} rij = {}", i, j, dij, rij);
                // INFO("i = {} k = {} dik = {} rik = {}", i, k, dik, rik);
                ASSERT_EQ(rij == rik, dij == dik);
                ASSERT_EQ(rij < rik, dij < dik);
                ASSERT_EQ(rij > rik, dij > dik);
            }
        }
    }
}


TEST(RankWeber, ton135) // > 280ms
{
    pse::TonIndex index(135);
    for (size_t i = 0; i < 135; ++i)
    {
        const pse::Ton& toni = index.ton(i);
        for (size_t j = 0; j < 135; ++j)
        {
            unsigned int dij = index.distWeber(i, j);
            // const pse::Ton& tonj = index.ton(j);
            EXPECT_EQ(dij, toni.distWeberModal(index.ton(j)));
            size_t rij = index.rankWeber(i, j);
            
            for (size_t k = 0; k < j; ++k)
            {
                unsigned int dik = index.distWeber(i, k);
                // const pse::Ton& tonk = index.ton(k);
                EXPECT_EQ(dik, toni.distWeberModal(index.ton(k)));
                size_t rik = index.rankWeber(i, k);
                // assert((rij == rik) == (dij == dik));
                // assert((rij  < rik) == (dij < dik));
                // assert((rij  > rik) == (dij > dik));
                
                // INFO("i = {} j = {} dij = {} rij = {}", i, j, dij, rij);
                // INFO("i = {} k = {} dik = {} rik = {}", i, k, dik, rik);
                ASSERT_EQ(rij == rik, dij == dik);
                ASSERT_EQ(rij < rik, dij < dik);
                ASSERT_EQ(rij > rik, dij > dik);
            }
        }
    }
}


TEST(RankWeber, ton165) // > 500ms
{
    pse::TonIndex index(165);
    for (size_t i = 0; i < 165; ++i)
    {
        const pse::Ton& toni = index.ton(i);
        for (size_t j = 0; j < 165; ++j)
        {
            unsigned int dij = index.distWeber(i, j);
            // const pse::Ton& tonj = index.ton(j);
            EXPECT_EQ(dij, toni.distWeberBluesModal(index.ton(j)));
            size_t rij = index.rankWeber(i, j);
            
            for (size_t k = 0; k < j; ++k)
            {
                unsigned int dik = index.distWeber(i, k);
                // const pse::Ton& tonk = index.ton(k);
                EXPECT_EQ(dik, toni.distWeberBluesModal(index.ton(k)));
                size_t rik = index.rankWeber(i, k);
                // assert((rij == rik) == (dij == dik));
                // assert((rij  < rik) == (dij < dik));
                // assert((rij  > rik) == (dij > dik));
                
                // INFO("i = {} j = {} dij = {} rij = {}", i, j, dij, rij);
                // INFO("i = {} k = {} dik = {} rik = {}", i, k, dik, rik);
                ASSERT_EQ(rij == rik, dij == dik);
                ASSERT_EQ(rij < rik, dij < dik);
                ASSERT_EQ(rij > rik, dij > dik);
            }
        }
    }
}

