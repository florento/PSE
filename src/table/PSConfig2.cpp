//
//  PSConfig2.cpp
//  pse
//
//  Created by Florent Jacquemard on 04/02/2023.
//
/// @addtogroup pitch
/// @{


#include "PSConfig2.hpp"

namespace pse {


// first step for processing chord
//PSC2::PSC2(const PSC0& c, const PSChord& e):
PSC2::PSC2(std::shared_ptr<const PSC0> c0, const PSC1c* c1, PSChord& chord):
PSC(c0),
_midis(chord.size(), 0),    // fix the size
_names(chord.size(), NoteName::Undef),
_accids(chord.size(), Accid::Undef),
_prints(chord.size(), false)
{
    assert(c0);
    assert(chord.size() > 1);
    _id = chord.stop();          // first note after chord
    assert(c1);
    *_cost += c1->cost();
    // _pred is c0
    const PSC0* co = c1;
    for (size_t i = chord.size()-1; i >= 0; --i)
    {
        const PSC1c* com = dynamic_cast<const PSC1c*>(co);
        assert(com);
        assert(i < _midis.size());
        _midis[i] = com->midi();
        assert(i < _names.size());
        assert(defined(com->name()));
        _names[i] = com->name();
        assert(i < _accids.size());
        assert(defined(com->accidental()));
        _accids[i] = com->accidental();
        assert(i < _prints.size());
        _prints[i] = com->printed();

        co = co->previous(); // NULL if co is initial
        assert(co);
    }
}


// copy
PSC2::PSC2(const PSC2& rhs):
PSC(rhs),
_midis(rhs._midis),     // vector copy
_names(rhs._names),   // vector copy
_accids(rhs._accids), // vector copy
_prints(rhs._prints)  // vector copy
{ }


PSC2::~PSC2()
{
    TRACE("delete PSConfig2");
}


PSC2& PSC2::operator=(const PSC2& rhs)
{
    if (this != &rhs)
    {
        PSC::operator=(rhs);
        _midis   = rhs._midis;
        _names  = rhs._names;
        _accids = rhs._accids;
        _prints = rhs._prints;
    }
    return *this;
}


bool PSC2::operator==(const PSC2& rhs) const
{
    return (PSC::operator==(rhs) &&
            (_midis == rhs._midis) &&
            (_names  == rhs._names) &&
            (_accids == rhs._accids) &&
            (_prints == rhs._prints));
}


bool PSC2::operator!=(const PSC2& rhs) const
{
    return (! operator==(rhs));
}


size_t PSC2::size() const
{
    return _midis.size();
}


unsigned int PSC2::midi(size_t i) const
{
    assert(i < _midis.size());
    return _midis.at(i);
}


enum NoteName PSC2::name(size_t i) const
{
    assert(i < _names.size());
    return _names.at(i);
}


std::vector<enum NoteName>::const_iterator PSC2::cbeginName() const
{
    return _names.cbegin();
}


std::vector<enum NoteName>::const_iterator PSC2::cendName() const
{
    return _names.cend();
}


enum Accid PSC2::accidental(size_t i) const
{
    assert(i < _accids.size());
    return _accids.at(i);
//    const enum NoteName& n = name(i);
//    assert(n != NoteName::Undef);
//    return _state.accid(n);
}


bool PSC2::printed(size_t i) const
{
    assert(i < _prints.size());
    return _prints.at(i);
}


std::vector<bool>::const_iterator PSC2::cbeginPrint() const
{
    return _prints.cbegin();
}


std::vector<bool>::const_iterator PSC2::cendPrint() const
{
    return _prints.cend();
}


bool PSC2::fromNote() const
{
    return false;
}


bool PSC2::fromChord() const
{
    return true;
}


//const PSState& PSC2::prevState() const
//{
//    assert(previous());
//    return previous()->state();
//}



} // end namespace pse

/// @}
