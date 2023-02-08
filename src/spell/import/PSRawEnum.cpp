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
_names(new std::vector<NoteName>),
_accids(new std::vector<Accid>),
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
PSEnum(e),
_notes(e._notes),  // vector copy (same vector elements)
_barnum(e._barnum),
_simult(e._simult),
_names(e._names),
_accids(e._accids),
_octs(e._octs),
_prints(e._prints)
{ }


std::unique_ptr<pse::PSEnum> PSRawEnum::clone() const
{
    return std::make_unique<PSRawEnum>(*this); // copy
}


PSRawEnum::PSRawEnum(const PSRawEnum& e, size_t i0):
PSEnum(e, i0),
_notes(e._notes),  // vector copy (same vector object)
_barnum(e._barnum),
_simult(e._simult),
_names(e._names),
_accids(e._accids),
_octs(e._octs),
_prints(e._prints)
{ }


std::unique_ptr<pse::PSEnum> PSRawEnum::clone(size_t i0) const
{
    return std::make_unique<PSRawEnum>(*this, i0);
}


PSRawEnum::PSRawEnum(const PSRawEnum& e, size_t i0, size_t i1):
PSEnum(e, i0, i1),
_notes(e._notes),  // vector copy (same vector object)
_barnum(e._barnum),
_simult(e._simult),
_names(e._names),
_accids(e._accids),
_octs(e._octs),
_prints(e._prints)
{ }


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
    if (_names == nullptr)        return false;
    if (_accids == nullptr)       return false;
    if (_octs == nullptr)         return false;
    if (_barnum->size() != _notes->size()) return false;
    if (_simult->size() != _notes->size()) return false;
    if (_names->size()  != _notes->size()) return false;
    if (_accids->size() != _notes->size()) return false;
    if (_octs->size()   != _notes->size()) return false;
    if (_prints->size() != _notes->size()) return false;
    return true;
}


size_t PSRawEnum::size() const
{
    assert(sanity_check());
    return _notes->size();
}


void PSRawEnum::add(int note, int bar, bool simult)
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

    // pad the output values (note names)
    assert(_names);
    _names->push_back(NoteName::Undef);
    assert(_accids);
    _accids->push_back(Accid::Undef);
    assert(_octs);
    _octs->push_back(OCTAVE_UNDEF);
    assert(_prints);
    _prints->push_back(false);

    _stop++;
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


pse::NoteName PSRawEnum::name(size_t i) const
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


pse::Accid PSRawEnum::accidental(size_t i) const
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


void PSRawEnum::rename(size_t i, const pse::NoteName& name,
                       const pse::Accid& accid, int oct, bool altprint)
{
    assert(sanity_check());
    // const py::object& pynote = _import[i];
    
    // possible to call non const methods on pynote (dirty) ?
    assert(name != NoteName::Undef);
    assert(_names);
    assert(i < _names->size());
    if (_names->at(i) != NoteName::Undef)
    {
        WARN("PSRawEnum overwriting {} with {} (was {})",
             i, name, _names->at(i));
    }
    _names->at(i) = name;
    
    assert(accid != Accid::Undef);
    assert(_accids);
    assert(i < _accids->size());
    if (_accids->at(i) != Accid::Undef)
    {
        WARN("PSRawEnum overwriting {} with {} (was {})",
             i, accid, _accids->at(i));
    }
    _accids->at(i) = accid;
    
    assert(oct != OCTAVE_UNDEF);
    assert(_octs);
    assert(i < _octs->size());
    if (_octs->at(i) != OCTAVE_UNDEF)
    {
        WARN("PSRawEnum overwriting {} with {} (was {})", i, oct, _octs->at(i));
    }
    _octs->at(i) = oct;
    
    assert(_prints);
    assert(i < _prints->size());
    _prints->at(i) = altprint;
}


} // namespace scoremodel

