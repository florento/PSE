//
//  PSE.cpp
//  pse
//
//  Created by Florent Jacquemard on 29/03/2023.
//

#include "PSE.hpp"
//#include "CostA.hpp"
#include "CostADplus.hpp"
#include "CostADlex.hpp"

namespace pse {


PSE::PSE(size_t nbTons, bool dflag):
Speller2Pass(Algo::PSE, nbTons, dflag)
{
//_table0(Algo::PSE, _index, _enum, dflag)
// init table with default vector of tons
//    if (finit)
//    {
//        for (auto ton : TONS) _tons.push_back(ton); // vector copy default tons
//        _rowcost.assign(nbtons(), 0);
//        _frowcost.assign(nbtons(), false);
//        bool status = init();
//        if (status == false)
//        {
//            ERROR("PST: fail to compute spelling table {}-{}",
//                  _enum.first(), _enum.stop());
//        }
//    }
}


PSE::~PSE()
{
    TRACE("delete PSE");
}


//Algo PSE::algo() const
//{
//    return Algo::PSE;
//}



/// @todo add steps rewrite passing notes
bool PSE::spell()
{
    //DEBUGU("Speller respell: nb tonalities in table: {}", _table.nbTons());
    if (_index.size() == 0)
    {
        /// reset to default
        /// @todo mv to speller cstr?
        WARN("Speller respell: no tonality added, use default 30 tonality array");
        for (int ks = -7; ks <= 7; ++ks)
            _index.add(ks, ModeName::Major);
        for (int ks = -7; ks <= 7; ++ks)
            _index.add(ks, ModeName::Minor);
    }

    //    if (finit)
    //    {
    //        for (auto ton : TONS) _tons.push_back(ton); // vector copy default tons
    //        _rowcost.assign(nbtons(), 0);
    //        _frowcost.assign(nbtons(), false);
    //    }

    CostADlex  seed0; // zero
    CostADplus seed1; // zero
    // diff0=0, diff1=0, rewrite_flag1=false
    return Speller2Pass::spell(seed0, seed1, 0, 0, false);
}




// TBR not used
//std::array<const PSState, PSV::NBTONS> PSV::ASTATES =
//{
//    PSState(TONS[0],  false),
//    PSState(TONS[1],  false),
//    PSState(TONS[2],  false),
//    PSState(TONS[3],  false),
//    PSState(TONS[4],  false),
//    PSState(TONS[5],  false),
//    PSState(TONS[6],  false),
//    PSState(TONS[7],  false),
//    PSState(TONS[8],  false),
//    PSState(TONS[9],  false),
//    PSState(TONS[10], false),
//    PSState(TONS[11], false),
//    PSState(TONS[12], false),
//    PSState(TONS[13], false),
//    PSState(TONS[14], false),
//    PSState(TONS[15], false),
//    PSState(TONS[16], false),
//    PSState(TONS[17], false),
//    PSState(TONS[18], false),
//    PSState(TONS[19], false),
//    PSState(TONS[20], false),
//    PSState(TONS[21], false),
//    PSState(TONS[22], false),
//    PSState(TONS[23], false),
//    PSState(TONS[24], false),
//    PSState(TONS[25], false),
//    PSState(TONS[26], false),
//    PSState(TONS[27], false),
//    PSState(TONS[28], false),
//    PSState(TONS[29], false)
//};


// TBR not used
//std::array<const PSState, PSV::NBTONS> PSV::AJOKER =
//{
//    PSState(TONS[0],  true),
//    PSState(TONS[1],  true),
//    PSState(TONS[2],  true),
//    PSState(TONS[3],  true),
//    PSState(TONS[4],  true),
//    PSState(TONS[5],  true),
//    PSState(TONS[6],  true),
//    PSState(TONS[7],  true),
//    PSState(TONS[8],  true),
//    PSState(TONS[9],  true),
//    PSState(TONS[10], true),
//    PSState(TONS[11], true),
//    PSState(TONS[12], true),
//    PSState(TONS[13], true),
//    PSState(TONS[14], true),
//    PSState(TONS[15], true),
//    PSState(TONS[16], true),
//    PSState(TONS[17], true),
//    PSState(TONS[18], true),
//    PSState(TONS[19], true),
//    PSState(TONS[20], true),
//    PSState(TONS[21], true),
//    PSState(TONS[22], true),
//    PSState(TONS[23], true),
//    PSState(TONS[24], true),
//    PSState(TONS[25], true),
//    PSState(TONS[26], true),
//    PSState(TONS[27], true),
//    PSState(TONS[28], true),
//    PSState(TONS[29], true)
//};


} // namespace pse
