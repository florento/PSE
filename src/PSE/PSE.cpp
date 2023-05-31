//
//  PSE.cpp
//  pse
//
//  Created by Florent Jacquemard on 29/03/2023.
//

#include "PSE.hpp"
#include "CostA.hpp"

namespace pse {


PSE::PSE(size_t nbTons, bool dflag):
Speller2Pass(Algo::PSE, nbTons, dflag)
{
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


//_table0(Algo::PSE, _index, _enum, dflag)



PSE::~PSE()
{
    TRACE("delete PSE");
}


//Algo PSE::algo() const
//{
//    return Algo::PSE;
//}




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
    
    bool status = true;

    //    if (finit)
    //    {
    //        for (auto ton : TONS) _tons.push_back(ton); // vector copy default tons
    //        _rowcost.assign(nbtons(), 0);
    //        _frowcost.assign(nbtons(), false);
    //    }
    if (_table0 != nullptr)
    {
        WARN("PSE: re-spelling, PS table overrided");
    }
    
    TRACE("pitch-spelling: building first pitch-spelling table");
    CostA seed0; // zero
    _table0 = std::unique_ptr<PST>(new PST(Algo::PSE, seed0, _index, _enum,
                                           _debug));
       
    TRACE("pitch-spelling: {} bars", _table0->size());

    TRACE("pitch-spelling: estimate first list of global tonality candidates");
    _global0 = std::unique_ptr<PSO>(new PSO(*_table0, 0, _debug));

    // extract local tonality for each column of table
    TRACE("pitch-spelling: start local tonalities estimation");
    _locals0 = std::unique_ptr<PSG>(new PSG(*_table0, _global0->getMask()));

    if (status == false)
    {
        ERROR("Pitch Spelling: failed to extract local tonalities, abort.");
        return false;
    }

    TRACE("pitch-spelling: building second pitch-spelling table");
    CostA seed1; // zero
    _table1 = std::unique_ptr<PST>(new PST(*_table0, seed1, *_locals0, _debug));

    TRACE("pitch-spelling: estimate second list of global tonality candidates");
    _global1 = std::unique_ptr<PSO>(new PSO(*_table1, 0, _debug));

    TRACE("Pitch Spelling: estimated global tonality: {} ({})",
          global(), global().fifths());


    // will update the lists _names, _accids and _octave
    TRACE("pitch-spelling: start renaming");
    _table1->rename(iglobal());

    if (status == false)
    {
        ERROR("Speller: failed to compute spelling table {}-{}",
              _enum.first(), _enum.stop());
    }
    return status;
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
