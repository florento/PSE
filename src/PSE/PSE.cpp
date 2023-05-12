//
//  PSE.cpp
//  pse
//
//  Created by Florent Jacquemard on 29/03/2023.
//

#include "PSE.hpp"


namespace pse {


PSE::PSE(size_t nbTons, bool dflag):
Speller(Algo::PSE, dflag),
_table(_enum, Algo::PSE, nbTons, dflag)
//_global(0, ModeName::Maj), // C maj default
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


PSE::~PSE()
{
    TRACE("delete PSE");
}


//Algo PSE::algo() const
//{
//    return Algo::PSE;
//}


size_t PSE::nbTons() const
{
    return _table.index.size();  // nbTons();
}


const Ton& PSE::ton(size_t i) const
{
    return _table.index.ton(i);
}


void PSE::resetTons()
{
    _table.index.reset();
}


void PSE::addTon(const Ton& ton)
{
    TRACE("Speller: add tonality {}", ton);
    _table.index.add(ton);
}


void PSE::addTon(int ks, ModeName mode)
{
    if (ks < -7 || 7 < ks)
    {
        ERROR("Speller addTon: wrong key signature value {}", ks);
    }
    TRACE("Speller: add tonality {} {}", ks, mode);
    _table.index.add(ks, mode);
}


void PSE::setGlobal(size_t i)
{
    _table.setGlobal(i);
}


bool PSE::spell()
{
    //DEBUGU("Speller respell: nb tonalities in table: {}", _table.nbTons());
    if (_table.index.size() == 0)
    {
        TRACE("Speller respell: no tonality added, use default tonality array");
        for (int ks = -7; ks <= 7; ++ks)
            _table.index.add(ks, ModeName::Major);
        for (int ks = -7; ks <= 7; ++ks)
            _table.index.add(ks, ModeName::Minor);
    }
    
    //    if (finit)
    //    {
    //        for (auto ton : TONS) _tons.push_back(ton); // vector copy default tons
    //        _rowcost.assign(nbtons(), 0);
    //        _frowcost.assign(nbtons(), false);
    //    }

    TRACE("pitch-spelling: building pitch-spelling table");
    bool status = _table.init();
    if (status == false)
    {
        ERROR("Speller: failed to compute spelling table {}-{}",
              _enum.first(), _enum.stop());
        return false;
    }

    TRACE("pitch-spelling: {} bars", _table.size());

    // estimate global tonality candidates from table
    if (! _table.estimatedGlobals())
    {
        TRACE("pitch-spelling: start estimation of global tonality candidates");
        status = _table.estimateGlobals();
        if (status == false)
        {
            ERROR("Pitch Spelling: failed to extract global tonality candidates, abort.");
            return false;
        }
    }
    
    // extract local tonality for each column of table
    TRACE("pitch-spelling: start local tonalities estimation");
    status = _table.estimateLocals();
    if (status == false)
    {
        ERROR("Pitch Spelling: failed to extract local tonalities, abort.");
        return false;
    }

    if (! _table.estimatedGlobal())
    {
        TRACE("pitch-spelling: start estimation of global tonality ");
        status = _table.estimateGlobal();
        if (status == false)
        {
            ERROR("Pitch Spelling: failed to extract global tonality, abort.");
            return false;
        }
    }

    TRACE("Pitch Spelling: estimated global tonality: {} ({})",
          _table.global(), _table.global().fifths());


    // will update the lists _names, _accids and _octave
    TRACE("pitch-spelling: start renaming");
    _table.rename();
    return true;
}


size_t PSE::globalCands() const
{
    return _table.globalCands();
}


const Ton& PSE::globalCand(size_t i) const
{
    return _table.globalCand(i);
}


size_t PSE::iglobalCand(size_t i) const
{
    return _table.iglobalCand(i);
}


size_t PSE::iglobal() const
{
    return _table.iglobal();
}


const Ton& PSE::global() const
{
    return _table.global();
}


const Ton& PSE::localCandBar(size_t i, size_t j) const
{
    return _table.local(i, j);
}


const Ton& PSE::localBar(size_t j) const
{
    return localCandBar(iglobal(), j);
}


const Ton& PSE::localNote(size_t i) const
{
    assert(_enum.inside(i));
    size_t j = _enum.measure(i);
    return localBar(j);
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
