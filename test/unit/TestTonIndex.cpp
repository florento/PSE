//
//  TestTonIndex.cpp
//  testpse
//
//  Created by Florent Jacquemard on 01/06/2023.
//

#include "gtest/gtest.h"

#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"
#include "TonIndex.hpp"


TEST(TonIndex, 0)
{
    pse::TonIndex id;  // = id(0)
    EXPECT_EQ(id.size(), 0);
    EXPECT_TRUE(id.empty());
}

TEST(TonIndex, 26_ks)
{
    pse::TonIndex id(26);
    EXPECT_EQ(id.size(), 26);
    EXPECT_FALSE(id.empty());
    
    EXPECT_EQ(id.ton(0),  pse::Ton(-6, pse::ModeName::Major));
    EXPECT_EQ(id.ton(1),  pse::Ton(-5, pse::ModeName::Major));
    EXPECT_EQ(id.ton(2),  pse::Ton(-4, pse::ModeName::Major));
    EXPECT_EQ(id.ton(3),  pse::Ton(-3, pse::ModeName::Major));
    EXPECT_EQ(id.ton(4),  pse::Ton(-2, pse::ModeName::Major));
    EXPECT_EQ(id.ton(5),  pse::Ton(-1, pse::ModeName::Major));
    EXPECT_EQ(id.ton(6),  pse::Ton( 0, pse::ModeName::Major));
    EXPECT_EQ(id.ton(7),  pse::Ton( 1, pse::ModeName::Major));
    EXPECT_EQ(id.ton(8),  pse::Ton( 2, pse::ModeName::Major));
    EXPECT_EQ(id.ton(9),  pse::Ton( 3, pse::ModeName::Major));
    EXPECT_EQ(id.ton(10), pse::Ton( 4, pse::ModeName::Major));
    EXPECT_EQ(id.ton(11), pse::Ton( 5, pse::ModeName::Major));
    EXPECT_EQ(id.ton(12), pse::Ton( 6, pse::ModeName::Major));

    EXPECT_EQ(id.ton(13), pse::Ton(-6, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(14), pse::Ton(-5, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(15), pse::Ton(-4, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(16), pse::Ton(-3, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(17), pse::Ton(-2, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(18), pse::Ton(-1, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(19), pse::Ton( 0, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(20), pse::Ton( 1, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(21), pse::Ton( 2, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(22), pse::Ton( 3, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(23), pse::Ton( 4, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(24), pse::Ton( 5, pse::ModeName::Minor));
    EXPECT_EQ(id.ton(25), pse::Ton( 6, pse::ModeName::Minor));
}

TEST(TonIndex, 26_name)
{
    pse::TonIndex id(26);
    EXPECT_EQ(id.size(), 26);
    EXPECT_FALSE(id.empty());
    
    EXPECT_EQ(id.ton(0).getName(), pse::NoteName::G);  // -6
    EXPECT_EQ(id.ton(0).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(0).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(1).getName(), pse::NoteName::D);
    EXPECT_EQ(id.ton(1).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(1).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(2).getName(), pse::NoteName::A);
    EXPECT_EQ(id.ton(2).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(2).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(3).getName(), pse::NoteName::E);
    EXPECT_EQ(id.ton(3).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(3).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(4).getName(), pse::NoteName::B);
    EXPECT_EQ(id.ton(4).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(4).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(5).getName(), pse::NoteName::F);
    EXPECT_EQ(id.ton(5).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(5).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(6).getName(), pse::NoteName::C);   // 0
    EXPECT_EQ(id.ton(6).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(6).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(7).getName(), pse::NoteName::G);
    EXPECT_EQ(id.ton(7).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(7).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(8).getName(), pse::NoteName::D);
    EXPECT_EQ(id.ton(8).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(8).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(9).getName(), pse::NoteName::A);
    EXPECT_EQ(id.ton(9).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(9).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(10).getName(), pse::NoteName::E);
    EXPECT_EQ(id.ton(10).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(10).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(11).getName(), pse::NoteName::B);
    EXPECT_EQ(id.ton(11).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(11).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(12).getName(), pse::NoteName::F);  // 6
    EXPECT_EQ(id.ton(12).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(12).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(13).getName(), pse::NoteName::E);  // -6
    EXPECT_EQ(id.ton(13).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(13).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(14).getName(), pse::NoteName::B);
    EXPECT_EQ(id.ton(14).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(14).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(15).getName(), pse::NoteName::F);
    EXPECT_EQ(id.ton(15).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(15).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(16).getName(), pse::NoteName::C);
    EXPECT_EQ(id.ton(16).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(16).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(17).getName(), pse::NoteName::G);
    EXPECT_EQ(id.ton(17).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(17).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(18).getName(), pse::NoteName::D);
    EXPECT_EQ(id.ton(18).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(18).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(19).getName(), pse::NoteName::A);  // 0
    EXPECT_EQ(id.ton(19).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(19).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(20).getName(), pse::NoteName::E);
    EXPECT_EQ(id.ton(20).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(20).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(21).getName(), pse::NoteName::B);
    EXPECT_EQ(id.ton(21).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(21).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(22).getName(), pse::NoteName::F);
    EXPECT_EQ(id.ton(22).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(22).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(23).getName(), pse::NoteName::C);
    EXPECT_EQ(id.ton(23).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(23).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(24).getName(), pse::NoteName::G);
    EXPECT_EQ(id.ton(24).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(24).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(25).getName(), pse::NoteName::D);
    EXPECT_EQ(id.ton(25).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(25).getMode(), pse::ModeName::Minor);
}

TEST(TonIndex, 30_name)
{
    pse::TonIndex id(30);
    EXPECT_EQ(id.size(), 30);
    EXPECT_FALSE(id.empty());
    
    EXPECT_EQ(id.ton(0).getName(), pse::NoteName::C);  // -7
    EXPECT_EQ(id.ton(0).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(0).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(1).getName(), pse::NoteName::G);  // -6
    EXPECT_EQ(id.ton(1).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(1).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(2).getName(), pse::NoteName::D);
    EXPECT_EQ(id.ton(2).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(2).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(3).getName(), pse::NoteName::A);
    EXPECT_EQ(id.ton(3).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(3).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(4).getName(), pse::NoteName::E);
    EXPECT_EQ(id.ton(4).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(4).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(5).getName(), pse::NoteName::B);
    EXPECT_EQ(id.ton(5).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(5).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(6).getName(), pse::NoteName::F);
    EXPECT_EQ(id.ton(6).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(6).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(7).getName(), pse::NoteName::C);   // 0
    EXPECT_EQ(id.ton(7).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(7).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(8).getName(), pse::NoteName::G);
    EXPECT_EQ(id.ton(8).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(8).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(9).getName(), pse::NoteName::D);
    EXPECT_EQ(id.ton(9).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(9).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(10).getName(), pse::NoteName::A);
    EXPECT_EQ(id.ton(10).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(10).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(11).getName(), pse::NoteName::E);
    EXPECT_EQ(id.ton(11).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(11).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(12).getName(), pse::NoteName::B);
    EXPECT_EQ(id.ton(12).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(12).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(13).getName(), pse::NoteName::F);  // 6
    EXPECT_EQ(id.ton(13).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(13).getMode(), pse::ModeName::Major);

    EXPECT_EQ(id.ton(14).getName(), pse::NoteName::C);  // 7
    EXPECT_EQ(id.ton(14).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(14).getMode(), pse::ModeName::Major);

        
    EXPECT_EQ(id.ton(15).getName(), pse::NoteName::A);  // -7
    EXPECT_EQ(id.ton(15).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(15).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(16).getName(), pse::NoteName::E);  // -6
    EXPECT_EQ(id.ton(16).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(16).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(17).getName(), pse::NoteName::B);
    EXPECT_EQ(id.ton(17).getAccidental(), pse::Accid::Flat);
    EXPECT_EQ(id.ton(17).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(18).getName(), pse::NoteName::F);
    EXPECT_EQ(id.ton(18).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(18).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(19).getName(), pse::NoteName::C);
    EXPECT_EQ(id.ton(19).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(19).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(20).getName(), pse::NoteName::G);
    EXPECT_EQ(id.ton(20).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(20).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(21).getName(), pse::NoteName::D);
    EXPECT_EQ(id.ton(21).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(21).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(22).getName(), pse::NoteName::A);  // 0
    EXPECT_EQ(id.ton(22).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(22).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(23).getName(), pse::NoteName::E);
    EXPECT_EQ(id.ton(23).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(23).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(24).getName(), pse::NoteName::B);
    EXPECT_EQ(id.ton(24).getAccidental(), pse::Accid::Natural);
    EXPECT_EQ(id.ton(24).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(25).getName(), pse::NoteName::F);
    EXPECT_EQ(id.ton(25).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(25).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(26).getName(), pse::NoteName::C);
    EXPECT_EQ(id.ton(26).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(26).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(27).getName(), pse::NoteName::G);
    EXPECT_EQ(id.ton(27).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(27).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(28).getName(), pse::NoteName::D);  // 6
    EXPECT_EQ(id.ton(28).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(28).getMode(), pse::ModeName::Minor);

    EXPECT_EQ(id.ton(29).getName(), pse::NoteName::A);  // 7
    EXPECT_EQ(id.ton(29).getAccidental(), pse::Accid::Sharp);
    EXPECT_EQ(id.ton(29).getMode(), pse::ModeName::Minor);

}
