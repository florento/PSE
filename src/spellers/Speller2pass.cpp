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


} // namespace pse
