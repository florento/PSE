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
#include <stdlib.h>     /* abs */

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


size_t PSEnum::size() const
{
    if (_stop != ID_INF)
        return (_stop - _first);
    else
    {
        WARN("PSEnum size: definition interval right open");
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


size_t PSEnum::count(int c, size_t i, size_t pre, size_t post) const
{
    //WARN("cound called on abstract PSEnum");     return 0;
    assert(0 <= c);
    assert(c < 12);
    // assert(! open());
    assert(! empty());
    size_t efirst = first();
    size_t estop = open()?first()+size():stop();
    assert(efirst <= i);
    assert(i < estop);
    // bounds of the interval to explore [i-pre, i+post)
    size_t left = (i - efirst >= pre)?(i - pre):efirst;
    size_t right = (estop - i >= post)?(i + post):estop;
    assert(left <= right);
    size_t cpt = 0;
    
    for (size_t j = left; j < right; ++j)
    {
        assert(efirst <= j);
        assert(j < estop);
        unsigned int mp = midipitch(j);
        assert(0 <= mp);
        assert(mp <= 128);
        if (mp%12 == c) ++cpt;
    }
    
    return cpt;
}

void PSEnum::rename(size_t i, const enum NoteName& n, bool altprint)
{
    const unsigned int m = midipitch(i);
    int o = MidiNum::midi_to_octave(m, n);
    enum Accid a = MidiNum::accid(m % 12, n);
    rename(i, n, a, o, altprint);
}

size_t PSEnum::rewritePassing()
{
    size_t ret = 0;
    size_t efirst = first();
    // size_t estop = open()?stop():efirst+size();
    // for (size_t i = efirst; i < estop; ++i)
    for (size_t i = efirst; inside(i+2); ++i)
    {
        bool rew = rewritePassing(i);
        ret += rew?1:0;
    }
    return ret;
}

bool PSEnum::rewritePassing(size_t i)
{
    // size_t efirst = first();
    // size_t estop = open()?stop():efirst+size();
    assert(first() <= i);
    // assert(i < estop);

    // not a trigram inside at i
    // if (estop - i < 3) return false;
    if (outside(i) || outside(i+1) || outside(i+2))
        return false;
    DEBUG("rewritePassing {}", i);

    int d0 = ((int) midipitch(i+1)) - ((int) midipitch(i));
    int d1 = ((int) midipitch(i+2)) - ((int) midipitch(i+1));

    if (d0 == 0 || d1 == 0) return false;
    if (abs(d0) > 2 || abs(d1) > 2) return false;

    const enum NoteName n0 = name(i);
    const enum NoteName n1 = name(i+1);
    const enum NoteName n2 = name(i+2);
    assert(n0 != NoteName::Undef);
    assert(n1 != NoteName::Undef);
    assert(n2 != NoteName::Undef);

//    if (n0 == NoteName::Undef)
//    {
//        ERROR("rewritePassing: {} undef", n0);
//        return false;
//    }
//
//    if (n1 == NoteName::Undef || n2 == NoteName::Undef)
//        return false;

    /// @todo TBC: overlapping cases?
    
    // broderie down
    if (d0 == -1 & d1 == 1 && n2 == n0 && n1 == n0)
    {
        rename(i+1, n1-1);
        return true;
    }

    // broderie up
    else if (d0 == 1 & d1 == -1 && n2 == n0 && n1 == n0)
    {
        rename(i+1, n1+1);
        return true;
    }

    // descending 1+
    else if ((d0 == -1 || d0 == -2) && d1 == -2 && n1 == n0 && n2 == n0-2)
    {
        rename(i+1, n1-1);
        return true;
    }

    // ascending 1+
    else if ((d0 == 1 || d0 == 2) && d1 == 2 && n1 == n0 && n2 == n0+2)
    {
        rename(i+1, n1+1);
        return true;
    }

    // descending 2+
    else if (d0 == -2 && (d1 == -1 || d1 == -2) && n2 == n1 && n2 == n0-2)
    {
        rename(i+1, n1+1);
        return true;
    }

    // ascending 2+
    else if (d0 == 2 && (d1 == 1 || d1 == 2) && n2 == n1 && n2 == n0+2)
    {
        rename(i+1, n1-1);
        return true;
    }
        
    else
        return false;
}


} // end namespace pse

/// @}

