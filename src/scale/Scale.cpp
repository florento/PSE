//
//  Scale.cpp
//  pse
//
//  Created by Florent on 24/03/2023.
//

#include "Scale.hpp"
#include "MidiNum.hpp"


namespace pse {

Scale::Scale(const Mode& mode, int pc, const enum NoteName& name):
_mode(mode.name()),
_pcs(),
_names(),
_accids(),
_ks((major(mode)||minor(mode))?pc:0, !minor(mode))
{
    assert(0 <= pc);
    assert(pc <= 11);
    _pcs.push_back(pc); // first degree
    _names.push_back(name); // first degree
    for (size_t i = 0; i < mode.size()-1; ++i)
    {
        assert(mode.semitonDistance(i) > 0);
        int c = (pc + mode.semitonDistance(i)) % 12;
        enum NoteName n = name + mode.nameDistance(i);
        _pcs.push_back(c);
        _names.push_back(n);
        assert(MidiNum::accid(c, n) != Accid::Undef);
        _accids.push_back(MidiNum::accid(c, n));
    }
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
