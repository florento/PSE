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
PSC2::PSC2(std::shared_ptr<const PSC0> c, PSEnum& e, size_t i0):
PSC(c),
_chord(std::make_shared<const PSChord>(e, i0)),
_current(firstChroma()),             // jump to the first non-empty pitch class
_names(_chord->size(), NoteName::Undef), // fix size
_accids(_chord->size(), Accid::Undef),
_prints(_chord->size(), false)
{
    assert(e.size() > 1);
    _id = _chord->stop();          // first note after chord
    // TBC no update of _cost?
}


PSC2::PSC2(const PSC2& c, const PSEnum& e,
           const enum NoteName& name, const enum Accid& accid,
           const Ton& ton):
PSC2(c) // copy of current
{
    assert(consistent(name, accid));
    
    // process the current pitch class (chroma)
    bool print = _state.update(name, accid);
    size_t nbocc = setNames(name, accid, print);
    // the given accidental corresponds to the chroma of input note and given name.
    assert(accid == MidiNum::accid(c.current(), name));

    // update cost
    assert(_cost);
    _cost->update(*this, e, name, accid, print, nbocc, ton);
    _current = nextChroma();
}


PSC2::PSC2(const PSC2& c, const PSEnum& e,
           const enum NoteName& name, const enum Accid& accid,
           const Ton& ton, const Ton& lton):
PSC2(c) // copy of current
{
    assert(consistent(name, accid));
    
    // process the current pitch class (chroma)
    bool print = _state.update(name, accid);
    size_t nbocc = setNames(name, accid, print);

    // the given accidental corresponds to the chroma of input note and given name.
    assert(accid == MidiNum::accid(c.current(), name));

    // update cost
    assert(_cost);
    _cost->update(*this, e, name, accid, print, nbocc, ton, lton);
    _current = nextChroma();
}


// copy
PSC2::PSC2(const PSC2& rhs):
PSC(rhs),
_chord(rhs._chord),     // shared ptr copy
_current(rhs._current),
_names(rhs._names),     // vector copy
_accids(rhs._accids),   // vector copy
_prints(rhs._prints)    // vector copy
{ }


PSC2::~PSC2()
{
    assert(_chord);
    TRACE("delete PSConfig2 for notes {}..{}",
          _chord->first(), _chord->stop());
}


PSC2& PSC2::operator=(const PSC2& rhs)
{
    if (this != &rhs)
    {
        PSC::operator=(rhs);
        _chord        = rhs._chord;   // shared ptr copy (const chord)
        _current      = rhs._current;
        _names        = rhs._names;
        _accids       = rhs._accids;
        _prints       = rhs._prints;
    }
    return *this;
}


bool PSC2::operator==(const PSC2& rhs) const
{
    assert(_chord);
    assert(rhs._chord);
    return (PSC::operator==(rhs) &&
            (_chord->first() == rhs._chord->first()) &&
            (_chord->stop()  == rhs._chord->stop()) &&
            (_current == rhs._current) &&
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
    assert(_chord);
    return _chord->size();
}


bool PSC2::complete() const
{
    return (_current >= 12); // _chord.length());
}


unsigned int PSC2::midi(size_t i) const
{
    assert(_chord);
    assert(i < size());
    return _chord->midipitch(_chord->first() + i);
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
    return _accids[i];
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


const PSState& PSC2::prevState() const
{
    assert(previous());
    return previous()->state();
}


unsigned int PSC2::firstChroma() const
{
    assert(_chord);
    for (unsigned int c = 0; c < 12; ++c)
    {
        if (_chord->occurences(c) > 0)
            return c;
    }
    // nothing in chord, should not happen
    ERROR("PSC2: empty chord {}-{}", _chord->first(), _chord->stop());
    return 12;
}


unsigned int PSC2::nextChroma() const
{
    assert(_chord);
    unsigned int c = _current+1;
    while (c < 12)
    {
        if (_chord->occurences(c) > 0)
            return c;
        c++;
    }
    assert(c >= 12);
    return 12;
}


bool PSC2::consistent(const enum NoteName& name, const enum Accid& accid) const
{
    assert(previous());
    // the accidental has not been changed for that name yet
    if (_state.accid(name) == previous()->state().accid(name))
        return true;
    // not two different accidentals for the same name in the chord
    else
        return (_state.accid(name) == accid);
}


size_t PSC2::setNames(const enum NoteName& name, const enum Accid& accid,
                      bool print)
{
    assert(_chord);
    assert(0 <= _current);
    assert(_current < 12);
    //assert(accid == MidiNum::accid(_current, name));
    
    // occurrences of notes in current pitch class
    size_t nbocc = _chord->occurences(_current);

    for (size_t i = 0; i < nbocc; ++i)
    {
        // index in enumerator (chord is an enumerator)
        size_t jo = _chord->occurence(_current, i);
        assert(_chord->first() <= jo);
        assert(jo < _chord->stop());
        size_t j = jo - _chord->first();
        assert(j < _names.size());
        _names[j]  = name;
        assert(j < _accids.size());
        _accids[j]  = accid;
        assert(j < _prints.size());
        _prints[j] = print;
    }
    return nbocc;
}


// @todo mv to PSCost:
// update(const PSC1& c, const PSEnum& e, const Ton& gton)
//bool PSC2::updateCost(const enum NoteName& name, const enum Accid& accid,
//                      bool print,
//                      size_t nbocc, const Ton& ton)
//{
//    // whether the cost has to be changed
//    bool cc = false;
//    if (ton.lead(name)) // sensible
//        cc = print && (ton.accidDia(name) != accid);
//    else
//        cc = print;
//
//    if (cc)
//    {
//        switch (accid)
//        {
//            case Accid::DoubleSharp:
//            case Accid::DoubleFlat:
//                _cost.incrAccid(2*nbocc);
//                break;
//
//            case Accid::Sharp:
//            case Accid::Flat:
//                _cost.incrAccid(nbocc);
//                break;
//
//            case Accid::Natural:
//                _cost.incrAccid(nbocc);
//                break;
//
//            default:
//            {
//                ERROR("PSC: unexpected accidental"); // accid
//                break;
//            }
//        }
//    }
//
//    return cc;
//}


// @todo mv to PSCost:
// update(const PSC1& c, const PSEnum& e, const Ton& gton, const Ton& lton)
//bool PSC2::updateCost(const enum NoteName& name, const enum Accid& accid,
//                      bool print,
//                      size_t nbocc, const Ton& ton, const Ton& lton)
//{
//    // update number of accid
//    bool res = updateCost(name, accid, print, nbocc, ton);;
//
//    // complete the update
//
//    // distance to conjectured local ton.
//    _cost.incrDist(_state.dist(lton));
//
//    // no update of disjoint move _disj
//
//    // color of accident and color of global ton
//    if (((ton.fifths() >= 0) && (flat(accid))) ||
//        ((ton.fifths() < 0) && (sharp(accid))))
//    {
//        _cost.incrColor(nbocc);
//        res = true;
//    }
//
//    return res;
//}

} // end namespace pse

/// @}
