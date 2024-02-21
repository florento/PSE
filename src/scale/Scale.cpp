//
//  Scale.cpp
//  pse
//
//  Created by Florent on 24/03/2023.
//

#include "Scale.hpp"
#include "NoteName.hpp"
#include "Ton.hpp"

namespace pse {

Scale::Scale():
_mode(ModeName::Undef),
_pcs(),
_names(),
_accids(),
_ks(0)
{ }


Scale::Scale(const ModeName& modename, int pc, const enum NoteName& name):
Scale(Mode(modename), pc, name)
{ }


Scale::Scale(const Mode& mode, int pc, const enum NoteName& name):
_mode(mode.name()),
_pcs(),
_names(),
_accids(),
_ks(diatonic(mode)?pc:0) //  major(mode)  flag major is OBSOLETE
{
    assert(0 <= pc);
    assert(pc <= 11);
    // degree 0
    //_pcs.push_back(pc);
    //_names.push_back(name);
    // degree 0..last
    for (size_t d = 0; d < mode.size(); ++d)
    {
        assert(mode.semitonDistance(d) >= 0);
        int c = (pc + mode.semitonDistance(d)) % 12;
        _pcs.push_back(c);
        enum NoteName n = name + mode.nameDistance(d);
        _names.push_back(n);
        assert(MidiNum::accid(c, n) != Accid::Undef);
        _accids.push_back(MidiNum::accid(c, n));
    }
}


Scale::Scale(const Ton& ton):
Scale(ton.getMode(), ton.getPitchClass(), ton.getName())
{
    assert(diatonic(ton.getMode()));
    // @todo also for other modes?
    // assert((ton.getMode() == ModeName::Major) ||
    //        (ton.getMode() == ModeName::Minor) ||
    //        (ton.getMode() == ModeName::MinorNat) ||
    //        (ton.getMode() == ModeName::MinorMel));
}


Scale::Scale(const Ton& ton, const ModeName& mode):
Scale(ModeName::Chromatic, ton.getPitchClass(), ton.getName())
{
    assert(mode == ModeName::Chromatic);
    // @todo also for other modes?
    assert(diatonic(ton.getMode()));
    // @todo also for other modes?
    // assert ((ton.getMode() == ModeName::Major) ||
    //         (ton.getMode() == ModeName::Minor) ||
    //         (ton.getMode() == ModeName::MinorNat) ||
    //         (ton.getMode() == ModeName::MinorMel));
}


bool Scale::undef() const
{
    return (_mode == ModeName::Undef);
}


// static private
bool Scale::major(const Mode& mode)
{
    return (mode.name() == ModeName::Major);
}


// static private
bool Scale::minor(const Mode& mode)
{
    return ((mode.name() == ModeName::Minor) ||
            (mode.name() == ModeName::MinorNat) ||
            (mode.name() == ModeName::MinorMel));
}

// static private
bool Scale::diatonic(const Mode& mode)
{
    return (major(mode) ||
            minor(mode) ||
            (mode.name() == ModeName::Ionian) ||
            (mode.name() == ModeName::Dorian) ||
            (mode.name() == ModeName::Phrygian) ||
            (mode.name() == ModeName::Lydian) ||
            (mode.name() == ModeName::Mixolydian) ||
            (mode.name() == ModeName::Aeolian) ||
            (mode.name() == ModeName::Locrian));
}


size_t Scale::size() const
{
    assert(_names.size() == _accids.size());
    return _names.size();
}


enum NoteName Scale::name(size_t d) const
{
    assert(d < _names.size());
    return _names.at(d);
}


enum Accid Scale::accid(size_t d) const
{
//    return MidiNum::accid(pitchClass(d), name(d));
    assert(d < _accids.size());
    return _accids.at(d);
}


int Scale::pitchClass(size_t d) const
{
    assert(d < _pcs.size());
    return _pcs.at(d);
}


bool Scale::contains(const enum NoteName& n, const enum Accid& a) const
{
    assert(_names.size() == _accids.size());
    for (size_t d = 0; d < _names.size(); ++d)
    {
        if (_names.at(d) == n && _accids.at(d) == a)
            return true;
    }
    return false; // not found
}


int Scale::fifths() const
{
    return _ks.fifths();
}


KeyFifth Scale::keySignature() const
{
    return _ks;
}


std::ostream& operator<<(std::ostream& o, const Scale& s)
{
    /// TBC
    return o;
}



} // end namespace pse
