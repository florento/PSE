//
//  KeyFifth.cpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "KeyFifth.hpp"

namespace pse {

KeyFifth::KeyFifth(int signature):
_sig(signature)
{
    assert(-7 <= signature);
    assert(signature <= 10); // chromatic Ton uses -7 to 10
}


KeyFifth::KeyFifth(int c, bool major):
_sig(key(c, major))
{
    assert(0 <= c);
    assert(c <= 11);
    assert(-7 <= _sig);
    assert(_sig <= 7); // chromatic Ton uses -7 to 10
}


KeyFifth::KeyFifth(const KeyFifth& k):
_sig(k._sig)
{ }


KeyFifth& KeyFifth::operator=(const KeyFifth& rhs)
{
    if(this != &rhs)
    {
        _sig = rhs._sig;
    }

    return *this;
}


bool KeyFifth::operator==(const KeyFifth& rhs) const
{
    return (this->_sig == rhs._sig);
}


bool KeyFifth::operator!=(const KeyFifth& rhs) const
{
    return (! this->operator==(rhs));
}


size_t KeyFifth::sharps() const
{
    if (_sig > 0)
        return _sig;
    else
        return 0;
}


size_t KeyFifth::flats() const
{
        if (_sig < 0)
            return abs(_sig);
        else
            return 0;
}


// static
int KeyFifth::key(int c, bool major)
{
    assert(0 <= c);
    assert(c <= 11);
    
    int k = (major)?((c * 7) % 12):(((c * 7) - 3) % 12);

    if (k > 6)
        k = k - 12;
    
    return k;
}


void KeyFifth::print(std::ostream& o) const
{
    if ( _sig == 0 )
        o << "0";
    else if ( _sig > 0 )
        o << sharps() << "#";
    else
        o << flats() << "b";
}


std::ostream& operator<<(std::ostream& o, const KeyFifth& k)
{
    k.print(o);
    return o;
}


} // end namespace pse
