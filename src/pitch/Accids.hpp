//
//  Accids.hpp
//  pse
//
//  Created by Florent Jacquemard on 22/03/2024.
//
/// @addtogroup pitch
/// @{
///

#ifndef Accids_hpp
#define Accids_hpp

#include <iostream>
#include <assert.h>
#include <string>

#include "trace.hpp"
#include "Accidental.hpp"


namespace pse {

/// type for encoding zero or one or two alternative of accidentals.
/// for the representation of zero or two options for one name in non-diatonic scales.
typedef std::uint8_t accids_t;


class Accids
{
    
public:

    /// nullary (undef) accids alternative.
    static accids_t encode();

    /// encode the given accidental into a singleton accids alternative.
    /// @param a1 accidental. must not be Accid::Undef.
    static accids_t encode(const enum Accid a1);

    /// encode the two given accidentals as two accids alternatives.
    /// @param a1 first optional accidental.  must not be Accid::Undef.
    /// @param a2 second optional accidental.  must not be Accid::Undef.
    static accids_t encode(const enum Accid a1, const enum Accid a2);

    /// the first component of the given encoding.
    /// if p is nullary it is Accid::Undef.
    static enum Accid first(accids_t p);

    /// the second component of the given encoding.
    /// if p is nullary or singleton it is Accid::Undef.
    static enum Accid second(accids_t p);

    /// the encoding represents zero option (no accidental possible).
    static bool zero(accids_t p);

    /// the encoding represents only one option.
    static bool single(accids_t p);

    /// the encoding represents two options.
    static bool two(accids_t p);

    /// the given accidental is one of the two option in the given encoding.
    /// @param a accidental. must not be Accid::Undef.
    static bool contained(enum Accid a, accids_t p);

    /// display the encoding.
    static std::string string(accids_t p);
    
private:
        
    /// encoding
    static accids_t toint(enum Accid a);

    /// decoding
    static enum Accid ofint(accids_t a);
    
    /// first projection (encoding)
    static accids_t fst(accids_t p);
    
    /// second projection (encoding)
    static accids_t snd(accids_t p);

};




} // namespace pse

#endif /* Accids_hpp */

/// @}
