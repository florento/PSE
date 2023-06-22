//
//  PSConfig1c.cpp
//  pse
//
//  Created by Florent Jacquemard on 19/06/2023.
//
/// @addtogroup pitch
/// @{

#include "PSConfig1c.hpp"



namespace pse {


// initial
PSC1c::PSC1c(std::shared_ptr<const PSC0> c, PSChord& chord):
PSC1(PSC(c)),  // cast of c into PSC with itself as pred.
_pcn(), // empty
_pcp(), // empty
_complete(chord.empty())
{
    assert(c);
    _pcn.fill(NoteName::Undef);
    _pcp.fill(false);
}


//PSC1c::PSC1c(const PSC0& c, PSChord& chord):
//PSC1(PSC(c))
//{ }




// note with new pitch class
PSC1c::PSC1c(std::shared_ptr<const PSC1c> c,
             const PSChord& chord,
             const enum NoteName& name,
             const enum Accid& accid,
             bool force_print,
             const Ton& gton,
             const Ton& lton):
PSC1(c, chord, name, accid, force_print, gton, lton),
_pcn(c->_pcn),
_pcp(c->_pcp),
_complete(_id == chord.stop())
{
    assert(c);
    assert(_midi == chord.midipitch(c->id()));
    assert(defined(name));
    assert(defined(accid));
    assert(gton.defined());
    // update the internal store of pitch class deja vu
    unsigned int m = _midi % 12;
    //assert((force_print == false) == (_pcn[m] == NoteName::Undef));
    // first time m is seen in chord
    if (_pcn[m] == NoteName::Undef)
    {
        _pcn[m] = name;
        _pcp[m] = _print;
    }
}


// note with pitch class deja vu
//PSC1c::PSC1c(std::shared_ptr<const PSC1c> c,
//             const PSChord& chord,
//             const Ton& ton,
//             const Ton& lton):
//PSC1c(*c) // copy and set manually the PSC1 and PSC1c attributes
//{
//    assert(c);
//    _pred = c;
//    assert(ton.defined());
//    // set info on note read (in chord) and spelling)
//    //assert(_midi == chord.midipitch(c->id()));
//    _midi = chord.midipitch(c->id());
//    unsigned int m = _midi % 12;
//    _name = _pcn[m];
//    assert(defined(_name)); // assert(_name != NoteName::Undef);
//    const enum Accid& accid = _state.accid(_name);
//    assert(defined(accid));
//    assert(accid == MidiNum::accid(m, _name));
//    _print = true;
//
//    // update id in enumerator (chord)
//    _id = c->id()+1; // next note in chord
//    _complete = (_id == chord.stop());
//
//    // update cost
//    assert(_cost);
//    _cost->update(*this, chord, ton);
//}


PSC1c::PSC1c(const PSC1c& c):
PSC1(c),
_pcn(c._pcn),
_complete(c._complete)
{ }


PSC1c::~PSC1c()
{
    TRACE("delete PSConfig1 from note {}", _id);
    //    if (_pred) delete _pred;
}


PSC1c& PSC1c::operator=(const PSC1c& rhs)
{
    if (this != &rhs)
    {
        PSC1::operator=(rhs);
        _pcn  = rhs._pcn;
        _complete = rhs._complete;
    }
    return *this;
}


bool PSC1c::operator==(const PSC1c& rhs) const
{
    return (PSC1::operator==(rhs) &&
            (_pcn == rhs._pcn) &&
            (_complete == rhs._complete));
}


bool PSC1c::operator!=(const PSC1c& rhs) const
{
    return (! operator==(rhs));
}


enum NoteName PSC1c::dejavu(unsigned int pc) const
{
    assert(0 <= pc);
    assert(pc < 12);
    return _pcn[pc];
}


bool PSC1c::dejaprint(unsigned int pc) const
{
    assert(0 <= pc);
    assert(pc < 12);
    return _pcp[pc];
}


bool PSC1c::complete() const
{
    return _complete;
}



} // end namespace pse

/// @}
