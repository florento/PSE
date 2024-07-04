//
//  PSRawEnum.cpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//

#include "PSRawEnum.hpp"


namespace pse {

// static
int PSRawEnum::OCTAVE_UNDEF = 100;

PSRawEnum::PSRawEnum(size_t i0, size_t i1):
PSEnum(i0, i1), // empty
_notes(new std::vector<int>), // empty initial vector
_barnum(new std::vector<int>),
_simult(new std::vector<bool>),
_durations(new std::vector<PSRatio>),
_names(new std::vector<enum NoteName>),
_accids(new std::vector<enum Accid>),
_octs(new std::vector<int>),
_prints(new std::vector<bool>)
{ }


//PSRawEnum::PSRawEnum(const std::vector<int>& notes,
//                     const std::vector<int>& barnum):
//PSEnum(0),
//_import(py::cast(std::vector<py::object&>())), /// ugly @todo rm
//_notes(notes),  // copy
//_barnum(barnum) // copy
//{
//    assert(_notes.size() == _barnum.size());
//}



PSRawEnum::PSRawEnum(const PSRawEnum& e):
PSEnum(e), // copy
_notes(e._notes),  // shallow copy of pointer
_barnum(e._barnum),
_simult(e._simult),
_durations(e._durations),
_names(e._names),
_accids(e._accids),
_octs(e._octs),
_prints(e._prints)
//_notes(new std::vector<int>(*(e._notes))),  // vector copy (same vector elements)
//_barnum(new std::vector<int>(*(e._barnum))),
//_simult(new std::vector<bool>(*(e._simult))),
//_names(new std::vector<enum NoteName>(*(e._names))),
//_accids(new std::vector<enum Accid>(*(e._accids))),
//_octs(new std::vector<int>(*(e._octs))),
//_prints(new std::vector<bool>(*(e._prints)))
{
    assert(e._notes);
    assert(e._barnum);
    assert(e._simult);
    assert(e._durations);
    assert(e._names);
    assert(e._accids);
    assert(e._octs);
    assert(e._prints);
}


std::unique_ptr<pse::PSEnum> PSRawEnum::clone() const
{
    return std::make_unique<PSRawEnum>(*this); // copy
}


PSRawEnum::PSRawEnum(const PSRawEnum& e, size_t i0):
PSEnum(e, i0),
_notes(e._notes),  // shallow copy of pointer
_barnum(e._barnum),
_simult(e._simult),
_durations(e._durations),
_names(e._names),
_accids(e._accids),
_octs(e._octs),
_prints(e._prints)
{
    assert(e._notes);
    assert(e._barnum);
    assert(e._simult);
    assert(e._durations);
    assert(e._names);
    assert(e._accids);
    assert(e._octs);
    assert(e._prints);
}


std::unique_ptr<pse::PSEnum> PSRawEnum::clone(size_t i0) const
{
    return std::make_unique<PSRawEnum>(*this, i0);
}


PSRawEnum::PSRawEnum(const PSRawEnum& e, size_t i0, size_t i1):
PSEnum(e, i0, i1),
_notes(e._notes),  // shallow copy of pointer
_barnum(e._barnum),
_simult(e._simult),
_durations(e._durations),
_names(e._names),
_accids(e._accids),
_octs(e._octs),
_prints(e._prints)
{
    assert(e._notes);
    assert(e._barnum);
    assert(e._simult);
    assert(e._durations);
    assert(e._names);
    assert(e._accids);
    assert(e._octs);
    assert(e._prints);
}


std::unique_ptr<pse::PSEnum> PSRawEnum::clone(size_t i0, size_t i1) const
{
    return std::make_unique<PSRawEnum>(*this, i0, i1);
}


PSRawEnum::~PSRawEnum()
{
    TRACE("delete PSRawEnum {}-{}", first(), stop());
    // DO NOT clear vectors which are shared with other (embedding) enumerators
}


bool PSRawEnum::sanity_check() const
{
    if (_notes == nullptr)        return false;
    if (_barnum == nullptr)       return false;
    if (_simult == nullptr)       return false;
    if (_durations == nullptr)    return false;
    if (_names == nullptr)        return false;
    if (_accids == nullptr)       return false;
    if (_octs == nullptr)         return false;
    if (_barnum->size() != _notes->size()) return false;
    if (_simult->size() != _notes->size()) return false;
    if (_durations->size() != _notes->size()) return false;
    if (_names->size()  != _notes->size()) return false;
    if (_accids->size() != _notes->size()) return false;
    if (_octs->size()   != _notes->size()) return false;
    if (_prints->size() != _notes->size()) return false;
    return true;
}


size_t PSRawEnum::size() const
{
    assert(sanity_check());
    if (open())
        return _notes->size() - first();
    else
        return stop() - first();
}


void PSRawEnum::reset(size_t i0, size_t i1)
{
    PSEnum::reset(i0, i1); // reset bounds
    assert(_notes);
    _notes->clear();
    assert(_barnum);
    _barnum->clear();
    assert(_simult);
    _simult->clear();
    assert(_durations);
    _durations->clear();
    assert(_names);
    _names->clear();
    assert(_accids);
    _accids->clear();
    assert(_octs);
    _octs->clear();
    assert(_prints);
    _prints->clear();
}


void PSRawEnum::add(int note, int bar, bool simult, const PSRatio& dur)
{
    assert(sanity_check());

    // note is a MIDI key
    assert(0 <= note);
    assert(note <= 128);
    assert(_notes);
    _notes->push_back(note);

    // next bar number
    assert(0 <= bar);
    assert(_barnum);
    assert(_barnum->empty() || (_barnum->back() <= bar));
    _barnum->push_back(bar);

    // simultaneous with next note
    assert(_simult);
    _simult->push_back(simult);

    // duration
    assert(_durations);
    _durations->push_back(dur);
    
    // pad the output values (note names)
    assert(_names);
    _names->push_back(NoteName::Undef);
    assert(_accids);
    _accids->push_back(Accid::Undef);
    assert(_octs);
    _octs->push_back(OCTAVE_UNDEF);
    assert(_prints);
    _prints->push_back(false);

    if (! open() && (_notes->size() > _stop))
        _stop = _notes->size();
}


void PSRawEnum::addlong(int note, int bar, bool simult, long dur_num, long dur_den)
{
    add(note, bar, simult, PSRatio(dur_num, dur_den));
}


unsigned int PSRawEnum::midipitch(size_t i) const
{
    assert(_notes);
    assert(i < _notes->size());
    return _notes->at(i);
}


long PSRawEnum::measure(size_t i) const
{
    assert(_barnum);
    assert(i < _barnum->size());
    return _barnum->at(i);
}


bool PSRawEnum::simultaneous(size_t i) const
{
    assert(_simult);
    assert(i < _simult->size());
    return _simult->at(i);
}


long PSRawEnum::duration_num(size_t i) const
{
    assert(_durations);
    assert(i < _durations->size());
    return _durations->at(i).numerator();
}


long PSRawEnum::duration_den(size_t i) const
{
    assert(_durations);
    assert(i < _durations->size());
    return _durations->at(i).denominator();
}


enum NoteName PSRawEnum::name(size_t i) const
{
    assert(_names);
    if (i < _names->size())
        return _names->at(i);
    else
    {
        ERROR("Speller name: {} out of range", i);
        return pse::NoteName::Undef;
    }
}


enum Accid PSRawEnum::accidental(size_t i) const
{
    assert(_accids);
    if (i < _accids->size())
        return _accids->at(i);
    else
    {
        ERROR("Speller accid: {} out of range", i);
        return pse::Accid::Undef;
    }
}


int PSRawEnum::octave(size_t i) const
{
    assert(_octs);
    if (i < _octs->size())
        return _octs->at(i);
    else
    {
        ERROR("Speller octave: {} out of range", i);
        return 100;
    }
}


bool PSRawEnum::printed(size_t i) const
{
    assert(_prints);
    if (i < _prints->size())
        return _prints->at(i);
    else
    {
        ERROR("Speller print flag: {} out of range", i);
        return false;
    }
}


void PSRawEnum::rename(size_t i,
                       const enum NoteName& n, const enum Accid& a, int o,
                       bool altprint)
{
    assert(_notes);
    assert(i < _notes->size());
    // int m = MidiNum::to_midi(n, a, o);
    if (MidiNum::to_midi(n, a, o) != _notes->at(i))
    {
        ERROR("PSRawEnum: MIDI pitch {} cannot be named by {}{} {}",
              _notes->at(i), n, a, o);
        return;
    }

    assert(sanity_check());
    // const py::object& pynote = _import[i];
    // possible to call non const methods on pynote (dirty) ?
    assert(n != NoteName::Undef);
    assert(_names);
    assert(i < _names->size());
//    if (_names->at(i) != NoteName::Undef)
//        WARN("PSRawEnum overwriting {} with {} (was {})", i, n, _names->at(i));
    _names->at(i) = n;
    
    assert(a != Accid::Undef);
    assert(_accids);
    assert(i < _accids->size());
//    if (_accids->at(i) != Accid::Undef)
//        WARN("PSRawEnum overwriting {} with {} (was {})", i, a, _accids->at(i));
    _accids->at(i) = a;
    
    assert(o != OCTAVE_UNDEF);
    assert(_octs);
    assert(i < _octs->size());
//    if (_octs->at(i) != OCTAVE_UNDEF)
//        WARN("PSRawEnum overwriting {} with {} (was {})", i, o, _octs->at(i));
    _octs->at(i) = o;
    
    assert(_prints);
    assert(i < _prints->size());
    _prints->at(i) = altprint;
}


void PSRawEnum::rename(size_t i, const enum NoteName& n, bool altprint)
{
    int m = _notes->at(i);
    enum Accid a = MidiNum::accid(m%12, n);
    if (a == Accid::Undef)
    {
        ERROR("PSRawEnum: pitch {} cannot be named by {}", m, n);
        return;
    }
    int o = MidiNum::midi_to_octave(m, n, a);
    rename(i, n, a, o, altprint);
}


//size_t PSRawEnum::count(int c, size_t i, size_t pre, size_t post)
//{
//    assert(0 <= c);
//    assert(c < 12);
//    assert(! open());
//    assert(! empty());
//    size_t efirst = first();
//    size_t estop = stop();
//    assert(efirst <= i);
//    assert(i <= stop());
//    size_t left = (i - efirst >= pre)?(i - pre):efirst;
//    size_t right = (estop - i >= post)?(i + post):estop;
//    assert(left <= right);
//    size_t cpt = 0;
//
//    for (size_t j = left; j < right; ++j)
//    {
//        assert(efirst <= j);
//        assert(j < estop);
//        unsigned int mp = midipitch(j);
//        assert(0 <= mp);
//        assert(mp <= 128);
//        if (mp%12 == c) ++cpt;
//    }
//
//    return cpt;
//}


} // namespace scoremodel

