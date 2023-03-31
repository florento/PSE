//
//  Scale.hpp
//  pse
//
//  Created by Florent on 24/03/2023.
//

#ifndef Scale_hpp
#define Scale_hpp

#include <iostream>
#include <vector>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
//#include "Fifths.hpp"
#include "KeyFifth.hpp"
#include "ModeName.hpp"
#include "Mode.hpp"

namespace pse {

class Scale  // public Mode
{
public:
    /// main constructor
    /// @param mode mode of this scale
    /// @param pc pitch class of tonic, in 0..11
    /// @param name name of tonic, in A..G
    Scale(const Mode& mode, int pc, const enum NoteName& name);
    
    /// note name for given degree.
    /// @param deg degree in this scale. the first degree is 0 (not 1).
    /// @return the note name for the given degree, in A..G.
    enum NoteName name(size_t deg) const;

    /// accidental name for given degree.
    /// @param deg degree in this scale. the first degree is 0 (not 1).
    /// @return the accidentals for the given degree, in -2..2.
    enum Accid accid(size_t deg) const;

    /// pitch class for given degree
    /// @param deg degree in this scale. the first degree is 0 (not 1).
    /// @return number of pitch class in 0..11.
    int pitchClass(size_t deg) const;

    /// this scale contains the note of given name and accidental.
    /// @param n note name in 'A'..'G'.
    /// @brief a accidental in [-2, 2], where 1.0 is a half tone.
    bool contains(const enum NoteName& n, const enum Accid& a) const;

    /// distance to C maj in the line of fifths
    /// @return the key signature in -7..7
    int fifths() const;

    /// key signature for diatonic scales (0 for others).
    /// @return the key signature in -7..7
    KeyFifth keySignature() const;

protected:

    /// mode of scale
    ModeName _mode;
    
    /// pitch class of every degree
    std::vector<int> _pcs;
   
    /// name of every degree
    std::vector<enum NoteName> _names;

    /// accidental of every degree
    std::vector<enum Accid> _accids;

    /// key signature for diatonic scales (0 for others)
    KeyFifth _ks;

    // void init_ks();
    
    /// mode is major diatonic
    static bool major(const Mode& mode);

    /// mode is minor diatonic
    /// (harmonic minor or melodic minor or natural minor)
    static bool minor(const Mode& mode);

};


std::ostream& operator<<(std::ostream& o, const Scale& s);


} // end namespace pse

#endif /* Scale_hpp */

/// @} // end group pse

