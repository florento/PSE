//
//  PSEnum.cpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#include "PSEnum.hpp"


namespace pse {

// static
size_t const PSEnum::ID_INF = -1;

PSEnum::PSEnum(size_t i0, size_t i1):
_first(i0),
_stop(i1)
{
    assert(i0 != ID_INF);
    assert((i1 == ID_INF) || (i0 <= i1));
}


PSEnum::PSEnum(const PSEnum& e):
_first(e._first),
_stop(e._stop)
{
    assert(_first != ID_INF);
    assert((_stop == ID_INF) || (_first <= _stop));
}


PSEnum::PSEnum(const PSEnum& e, size_t i0, size_t i1):
PSEnum(e)
{
    assert(i0 != ID_INF);
    assert((i1 == ID_INF) || (i0 <= i1));
    _first = i0;
    _stop = i1;
}


PSEnum::~PSEnum()
{
    TRACE("delete PSEnum {}-{}", first(), stop());
}


void PSEnum::reset(size_t i0, size_t i1)
{
    assert(i0 != ID_INF);
    assert((i1 == ID_INF) || (i0 <= i1));
    _first = i0;
    _stop = i1;
}


bool PSEnum::open() const
{
    return (_stop == ID_INF);
}


size_t PSEnum::first() const
{
    return _first;
}


size_t PSEnum::stop() const
{
    return _stop;
}


size_t PSEnum::length() const
{
    if (_stop != ID_INF)
        return (_stop - _first);
    else
    {
        WARN("PSEnum size: def interval right open");
        return ID_INF;
    }
}


bool PSEnum::empty() const
{
    if (_stop == ID_INF)
        return false;
    else
        return (_first == _stop);
}


bool PSEnum::inside(size_t i) const
{
    if (i < _first)
        return false;

    if (open())
    {
        WARN("PSEnum: inside {} called for open enumerator", i);
        return true;
    }
    else
        return (i < _stop);
}


bool PSEnum::outside(size_t i) const
{
    return (! inside(i));
}


} // end namespace pse

/// @}

