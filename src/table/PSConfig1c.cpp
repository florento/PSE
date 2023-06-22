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
//PSC1c::PSC1c(std::shared_ptr<const PSC0> c, PSChord& chord):
//PSC1(PSC(c)),  // cast of c into PSC with itself as pred.
//_pcn(), // empty
//_pcp(), // empty
//_complete(chord.empty())
//{
//    assert(c);
//    _pcn.fill(NoteName::Undef);
//    _pcp.fill(false);
//}


//PSC1c::PSC1c(const PSC0& c, PSChord& chord):
//PSC1(PSC(c))
//{ }


PSC1c::PSC1c(std::shared_ptr<const PSC0> c,
             const PSEnum& e,
             const enum NoteName& name,
             const enum Accid& accid,
             const Ton& gton,
             const Ton& lton):
PSC1(c, e, name, accid, false, gton, lton),
_chord(new PSChord(e, c->id())),
_pcn(), // empty
_pcp(), // empty
_complete(_chord->empty())
{
    assert(c);
    assert(c->id() == _chord->first());
    assert(defined(accid));
    assert(gton.defined());
    _pcn.fill(NoteName::Undef);
    _pcp.fill(false);
    post_init(*c, *_chord);
}


// note with new pitch class
PSC1c::PSC1c(std::shared_ptr<const PSC1c> c,
             const enum NoteName& name,
             const enum Accid& accid,
             bool force_print,
             const Ton& gton,
             const Ton& lton):
PSC1(c, c->chord(), name, accid, force_print, gton, lton),
_chord(init_chord(c.get())), // pointer copy of chord currently processed
_pcn(c->_pcn),
_pcp(c->_pcp),
_complete(_id == _chord->stop())
{
    assert(c);
    assert(gton.defined());
    //assert((force_print == false) == (_pcn[m] == NoteName::Undef));
    post_init(*c, *_chord);
}


std::shared_ptr<const PSChord> PSC1c::init_chord(const PSC1c* c) const
{
    assert(c);
    assert(c->_chord);
    return c->_chord;
}

void PSC1c::post_init(const PSC0& c, const PSChord& chord)
{
    // c.id() is the note processed and _id is the next
    assert(chord.first() <= c.id());
    assert(c.id() < chord.stop());
    assert(_midi == chord.midipitch(c.id()));
    assert(defined(_name));
    // update the internal store deja vu of pitch class
    // if first time pitch class is seen in chord
    unsigned int m = _midi % 12;
    if (_pcn[m] == NoteName::Undef)
    {
        _pcn[m] = _name;
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


bool PSC1c::inChord() const
{
    return true;
}


const PSChord& PSC1c::chord() const
{
    assert(_chord);
    return *(_chord);
}



} // end namespace pse

/// @}
