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
PSC2::PSC2(std::shared_ptr<const PSC0> c0,
           std::shared_ptr<const PSC1c> c1,
           PSChord& chord):
PSC(c0), // copy the initial state
_midis(chord.size(), 0),    // fix the size
_names(chord.size(), NoteName::Undef),
//_accids(chord.size(), Accid::Undef),
_prints(chord.size(), false)
{
    assert(c0);
    assert(chord.size() > 1);
    assert(_midis.size() == chord.size());
    assert(_names.size() == chord.size());
    // assert(_accids.size() == chord.size());
    assert(_prints.size() == chord.size());

    _id = chord.stop();          // first note after chord
    assert(c1);
    *_cost += c1->cost();
    // _pred is c0

    /// rollback the process chords in configs PC1
    const PSC1c* pc1 = c1.get();;
    for (long i = chord.size()-1; i >= 0; --i)
    {
        assert(pc1);
        assert(i < _midis.size());
        _midis[i] = pc1->midi();
        assert(i < _names.size());
        assert(defined(pc1->name()));
        _names[i] = pc1->name();
        // @todo TBR _accids
        // assert(i < _accids.size());
        // assert(defined(pc1->accidental()));
        // _accids[i] = pc1->accidental();
        assert(i < _prints.size());
        _prints[i] = pc1->printed();

        //co = c1->previous(); // NULL if co is initial
        pc1 = dynamic_cast<const PSC1c*>(pc1->previous());
    }
}


// copy
PSC2::PSC2(const PSC2& rhs):
PSC(rhs),
_midis(rhs._midis),   // vector copy
_names(rhs._names),   // vector copy
// _accids(rhs._accids), // vector copy  /// @todo TBR _accids
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
        // _accids = rhs._accids;     /// @todo TBR _accids
        _prints = rhs._prints;
    }
    return *this;
}


bool PSC2::operator==(const PSC2& rhs) const
{
    return (PSC::operator==(rhs)); // &&
         // (_midis == rhs._midis) &&
         // (_names  == rhs._names) &&
         // (_accids == rhs._accids) &&
         // (_prints == rhs._prints));
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
    const enum NoteName& n(name(i));
    assert(n != NoteName::Undef);
    enum Accid accid = MidiNum::midi_to_accid(midi(i), n);
    assert(_state);
    assert(Accids::contained(accid, _state->accids(n)));
    return accid; // cast to float format for Pitch ?
    // @todo TBR _accids
    // assert(i < _accids.size());
    // return _accids.at(i);
}


int PSC2::octave(size_t i) const
{
    const enum NoteName& n(name(i));
    assert(n != NoteName::Undef);
    return MidiNum::midi_to_octave(midi(i), n);
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
