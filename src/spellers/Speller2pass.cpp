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
    return Speller1Pass::globalCands(_global1);
}


const Ton& Speller2Pass::global(size_t i) const
{
    return Speller1Pass::globalCand(i, _global1);
}


size_t Speller2Pass::iglobal(size_t i) const
{
    return Speller1Pass::iglobalCand(i, _global1);
}


bool Speller2Pass::spell(const Cost& seed0, const Cost& seed1,
                         double diff0, //bool rewrite_flag0,
                         double diff1,
                         bool rename_flag1,
                         bool rewrite_flag1)
{
    bool status = true;

    // rename_flag = false, rewrite_flag = false
    TRACE("pitch-spelling: building first pitch-spelling table");
    status = status && Speller1Pass::spell(seed0, diff0, false, false);
    
    TRACE("pitch-spelling: building second pitch-spelling table");
    if (_table1 != nullptr) // should not happen
    {
        WARN("PSE: re-spelling, 2d PS table overrided");
        delete _table1;
    }
    assert(_global0);
    assert(_locals0);
    // std::unique_ptr<PST>
    _table1 = new PST(*_table0, seed1, *_global0, *_locals0, _debug);

    TRACE("pitch-spelling: estimate second list of global tonality candidates");
    if (_global1 != nullptr) // should not happen
    {
        WARN("PSE: re-spelling, 2d PS global set overrided");
        delete _global1;
    }
    assert(_global0);
    // std::unique_ptr<PSO>
    _global1 = new PSO(*_global0, *_table1, diff1, _debug);
    
    TRACE("Pitch Spelling: {} estimated global tonality candidates", globals());
    
    if (rename_flag1)
    {
        size_t ig = iglobal(0);
        
        if (ig != TonIndex::UNDEF)
        {
            assert(ig < _index.size());
            TRACE("Pitch Spelling: estimated global tonality: {} ({})",
                  ig, _index.ton(ig).fifths());
            
            // will update the lists _names, _accids and _octave
            TRACE("pitch-spelling: renaming");
            rename(ig);
        }
        else
        {
            ERROR("Speller: failure with spelling table {}-{}",
                  _enum.first(), _enum.stop());
            status = false;
        }

    }

    if (rename_flag1 && rewrite_flag1)
    {
        INFO("pitch-spelling: rewrite passing notes");
        _table1->enumerator().rewritePassing();
    }

    return status;
}


bool Speller2Pass::rename(size_t n)
{
    return Speller1Pass::rename(_table1, n);
}



} // namespace pse
