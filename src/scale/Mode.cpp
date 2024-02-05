//
//  Mode.cpp
//  pse
//
//  Created by Florent on 23/03/2023.
//

#include "Mode.hpp"


namespace pse {

Mode::Mode():
_name(ModeName::Undef),
_semitons(),  // empty
_csemitons(),
_names(),
_cnames()
{ }


Mode::Mode(const ModeName& code):
Mode(code, predefined_semitons(code), predefined_names(code))
{
    assert(code != ModeName::Undef);
}


Mode::Mode(const ModeName& code,
           const std::vector<const int>& ls,
           const std::vector<const int>& ln):
_name(code),
_semitons(ls), // vector copy
_csemitons(),  // initially empty
_names(ln),
_cnames()
{
    assert(_semitons.size() == _names.size());
    // first degree (tonic)
    int cs = 0;
    int cn = 0;
    for (size_t i = 0; i < _semitons.size(); ++i)
    {
        _csemitons.push_back(cs);
        _cnames.push_back(cn);
        assert(_semitons.at(i) >= 0);
        cs += _semitons.at(i);
        assert(_names.at(i) >= 0);
        cn += _names.at(i);
    }
}


Mode::Mode(const Mode& rhs):
_semitons(rhs._semitons), // vector copy
_csemitons(rhs._csemitons),
_names(rhs._names),
_cnames(rhs._cnames)
{
    assert(rhs._semitons.size() == rhs._names.size());
    assert(rhs._semitons.size() == rhs._csemitons.size());
    assert(rhs._names.size() == rhs._cnames.size());
}


Mode::~Mode()
{
//    _semitons.clear();
//    _csemitons.clear();
//    _names.clear();
//    _cnames.clear();
}


size_t Mode::size() const
{
    return _semitons.size();
}


bool Mode::undef() const
{
    assert(_name != ModeName::Undef || size() == 0);
    return (_name == ModeName::Undef);
}


const int Mode::semitonDistance(size_t d) const
{
    assert(d < _csemitons.size());
    return _csemitons.at(d);
}


const int Mode::nameDistance(size_t d) const
{
    assert(d < _cnames.size());
    return _cnames.at(d);
}


std::vector<const int> Mode::predefined_semitons(const ModeName& name)
{
    switch (name)
    {
        case ModeName::Major:
            return std::vector<const int>{2, 2, 1, 2, 2, 2, 1};

        // harmonic minor
        case ModeName::Minor:
            return std::vector<const int>{2, 1, 2, 2, 1, 3, 1};

        // natural minor
        case ModeName::MinorNat:
            return std::vector<const int>{2, 1, 2, 2, 1, 2, 2};

        // melodic minor
        case ModeName::MinorMel:
            return std::vector<const int>{2, 1, 2, 2, 2, 2, 1};

        // Mode of C = Major
        case ModeName::Ionian:
            return std::vector<const int>{2, 2, 1, 2, 2, 2, 1};

        // Mode of D
        case ModeName::Dorian:
            return std::vector<const int>{2, 1, 2, 2, 2, 1, 2};

        // Mode of E
        case ModeName::Phrygian:
            return std::vector<const int>{1, 2, 2, 2, 1, 2, 2};
            
        // Mode of F
        case ModeName::Lydian:
            return std::vector<const int>{2, 2, 2, 1, 2, 2, 1};
            
        // Mode of G
        case ModeName::Mixolydian:
            return std::vector<const int>{2, 2, 1, 2, 2, 1, 2};

        // Mode of A
        case ModeName::Aeolian:
            return std::vector<const int>{2, 1, 2, 2, 1, 2, 2};

        // Mode of B
        case ModeName::Locrian:
            return std::vector<const int>{1, 2, 2, 1, 2, 2, 2};

        // Harmonic Chromatic
        // all notes are repeated except tonic and dominant
        case ModeName::Chromatic:
        return std::vector<const int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
                
        // Major Pentatonic = Major - 4th and 7th degrees
        /// @todo is it the correct spelling?  see https://muted.io/major-pentatonic-scale
        case ModeName::Major5:
            return std::vector<const int>{2, 2, 3, 2, 3};
            

        // Minor Pentatonic =  natural minor - 2nd and 6th degrees.
        case ModeName::Minor5:
            return std::vector<const int>{1, 2, 2, 1, 2};

        // = Major5 + flat 3d blue note
        /// @todo is it the correct spelling?  see https://muted.io/major-blues-scale
        case ModeName::MajorBlues:
            return std::vector<const int>{2, 1, 1, 3, 2, 3};

        // = Minor5 + flat 5th blue note
        /// @todo is it the correct spelling? see https://muted.io/minor-blues-scale
        case ModeName::MinorBlues:
            return std::vector<const int>{3, 2, 1, 1, 3, 2};

        // Augmented Scale = symmetrical augmented scale = two augmented triads
        /// @todo is it the correct spelling? see https://muted.io/augmented-scale
        case ModeName::Augmented:
            return std::vector<const int>{3, 1, 3, 1, 3, 1};


        // Diminished Scale = symmetric diminished scale = two diminished 7th chords
        /// @todo is it the correct spelling? not diatonic see https://muted.io/diminished-scale
        case ModeName::Diminished:
            return std::vector<const int>{2, 1, 2, 1, 2, 1, 2, 1};

        case ModeName::Undef:
        {
            ERROR("Mode: mode name should not be Undef");
            return std::vector<const int>(); // empty vector
        }
            
        default:
        {
            ERROR("Mode: unknown mode name {}", name);
            return std::vector<const int>();
        }
    }
}


std::vector<const int> Mode::predefined_names(const ModeName& name)
{
    switch (name)
    {
        case ModeName::Major:
        // harmonic minor
        case ModeName::Minor:
        // natural minor
        case ModeName::MinorNat:
        // melodic minor
        case ModeName::MinorMel:
        // Mode of C = Major
        case ModeName::Ionian:
        // Mode of D
        case ModeName::Dorian:
        // Mode of E
        case ModeName::Phrygian:
        // Mode of F
        case ModeName::Lydian:
        // Mode of G
        case ModeName::Mixolydian:
        // Mode of A
        case ModeName::Aeolian:
        // Mode of B
        case ModeName::Locrian:
            return std::vector<const int>{1, 1, 1, 1, 1, 1, 1};

        // Harmonic Chromatic
        // all notes are repeated except tonic and dominant
        case ModeName::Chromatic:
             return
            std::vector<const int>{1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1};
                
        // Major Pentatonic = Major - 4th and 7th degrees
        /// @todo is it the correct spelling?  see https://muted.io/major-pentatonic-scale
        case ModeName::Major5:
            return std::vector<const int>{1, 1, 2, 1, 2};

        // Minor Pentatonic =  natural minor - 2nd and 6th degrees.
        case ModeName::Minor5:
            return std::vector<const int>{2, 1, 1, 2, 1};

        // = Major5 + flat 3d blue note
        /// @todo is it the correct spelling?  see https://muted.io/major-blues-scale
        case ModeName::MajorBlues:
            return std::vector<const int>{1, 1, 1, 1, 1, 2};

        // = Minor5 + flat 5th blue note
        /// @todo is it the correct spelling? see https://muted.io/minor-blues-scale
        case ModeName::MinorBlues:
            return std::vector<const int>{2, 1, 1, 1, 0, 2};

        // Augmented Scale = symmetrical augmented scale = two augmented triads
        /// @todo is it the correct spelling? see https://muted.io/augmented-scale
        case ModeName::Augmented:
            return std::vector<const int>{1, 1, 2, 1, 2, 1};


        // Diminished Scale = symmetric diminished scale = two diminished 7th chords
        /// @todo is it the correct spelling? not diatonic see https://muted.io/diminished-scale
        case ModeName::Diminished:
            return std::vector<const int>{1, 1, 1, 1, 1, 0, 1, 1};

        case ModeName::Undef:
        {
            ERROR("Mode: mode name should not be Undef");
            return std::vector<const int>(); // empty vector
        }
            
        default:
        {
            ERROR("Mode: unknown mode name {}", name);
            return std::vector<const int>();
        }
    }
}

} // end namespace pse
