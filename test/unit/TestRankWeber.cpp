//
//  TestRankWeber.cpp
//  testpse
//
//  Created by Florent Jacquemard on 01/07/2023.
//

#include "gtest/gtest.h"

#include "TonIndex.hpp"
#include "Weber.hpp"

 
TEST(TestRankWeber, ton1)
{
    pse::TonIndex index;
    index.add(0, pse::ModeName::Major); // C maj
    index.close();
    ASSERT_EQ(index.rankWeber(0, 0), 0);
}


TEST(TestRankWeber, ton3)
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


TEST(TestRankWeber, ton30)
{
    pse::TonIndex index(30);
    for (size_t i = 0; i < 30; ++i)
    {
        const pse::Ton& toni = index.ton(i);
        for (size_t j = 0; j < 30; ++j)
        {
            const pse::Ton& tonj = index.ton(j);
            for (size_t k = 0; k < j; ++k)
            {
                const pse::Ton& tonk = index.ton(k);
                size_t rij = index.rankWeber(i, j);
                size_t rik = index.rankWeber(i, k);
                unsigned int dij = toni.distWeber(tonj);
                unsigned int dik = toni.distWeber(tonk);

                // assert((rij == rik) == (dij == dik));
                // assert((rij  < rik) == (dij < dik));
                // assert((rij  > rik) == (dij > dik));
                
                ASSERT_EQ(rij == rik, dij == dik);
                ASSERT_EQ(rij < rik, dij < dik);
                ASSERT_EQ(rij > rik, dij > dik);
            }
        }
    }
}
