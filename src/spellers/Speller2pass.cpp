//
//  Speller2pass.cpp
//  pse
//
//  Created by Florent Jacquemard on 31/05/2023.
//
/// @addtogroup pitch
/// @{

#include "Speller2pass.hpp"

namespace pse {


Speller2Pass::Speller2Pass(const Algo& algo, size_t nbTons, bool dflag):
Speller1Pass(algo, nbTons, dflag),
_table1(nullptr),
_global1(nullptr)
{ }


Speller2Pass::~Speller2Pass()
{
    if (_table1)
        delete _table1;
    if (_global1)
        delete _global1;
}


void Speller2Pass::setGlobal(size_t i)
{
    Speller1Pass::setGlobal(i, _global0);
    Speller1Pass::setGlobal(i, _global1);
}


size_t Speller2Pass::globals() const
{
    return Speller1Pass::globals(_global1);
}


const Ton& Speller2Pass::globalCand(size_t i) const
{
    return Speller1Pass::globalCand(i, _global1);
}


size_t Speller2Pass::iglobalCand(size_t i) const
{
    return Speller1Pass::iglobalCand(i, _global1);
}


bool Speller2Pass::spell(const Cost& seed0, const Cost& seed1,
                         double diff0, bool rewrite_flag0,
                         double diff1, bool rewrite_flag1)
{
    bool status = true;

    status = status && Speller1Pass::spell(seed0, diff0, rewrite_flag0);
    
    TRACE("pitch-spelling: building first pitch-spelling table");

    TRACE("pitch-spelling: building second pitch-spelling table");
    if (_table1 != nullptr)
    {
        WARN("PSE: re-spelling, 2d PS table overrided");
        delete _table1;
    }
    _table1 = new PST(*_table0, seed1, *_locals0, _debug); // std::unique_ptr<PST>

    TRACE("pitch-spelling: estimate second list of global tonality candidates");
    if (_global1 != nullptr)
    {
        WARN("PSE: re-spelling, 2d PS global set overrided");
        delete _global1;
    }
    _global1 = new PSO(*_table1, diff1, _debug); // std::unique_ptr<PSO>

    TRACE("Pitch Spelling: estimated global tonality: {} ({})",
          global(), global().fifths());

    if (rewrite_flag1)
    {
        TRACE("pitch-spelling: rewrite passing notes");
        _table1->enumerator().rewritePassing();
    }
    
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



} // namespace pse
