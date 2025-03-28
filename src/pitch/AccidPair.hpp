//
//  AccidPair.hpp
//  pse
//
//  Created by Florent Jacquemard on 19/03/2024.
//
/// @addtogroup pitch
/// @{

#ifndef AccidPair_hpp
#define AccidPair_hpp

#include <iostream>
#include <assert.h>
#include <string>

#include "pstrace.hpp"
#include "Accid.hpp"


namespace pse {


/// pair of accidental to represent two options for one name
/// in non-diatonic scales.
class AccidPair
{
    
public:
    
    /// @param a1 first optional accidental.
    /// can be Accid::Undef
    /// @param a2 second optional accidental.
    /// can be Accid::Undef
    /// must be Accid::Undef if _first is Accid::Undef
    /// otherwise, must be larger than _first.
    AccidPair(const enum Accid& a1=Accid::Undef,
              const enum Accid& a2=Accid::Undef);

    /// assignement
    AccidPair& operator=(const AccidPair& rhs);

    /// comparison
    bool operator==(const AccidPair& rhs) const;

    /// comparison
    bool operator!=(const AccidPair& rhs) const;

    /// first optional accidental.
    enum Accid first() const;

    /// first optional accidental.
    enum Accid second() const;

    /// this accid pair is undefined (no accidental possible).
    bool undef() const;

    /// this accid pair is not undefined (at least one accidental possible).
    bool defined() const;

    /// this accid pair contains only one option.
    bool single() const;

    /// this accid pair contains two options.
    bool pair() const;

    /// the given accidental is one of the two option.
    /// @param a accidental. must not be Accid::Undef.
    bool contains(const enum Accid& a) const;

    /// display this pair.
    void print(std::ostream& o) const;
    
private: // data

    /// first optional accidental.
    enum Accid _first;

    /// second optional accidental.
    /// can be Accid::Undef
    /// must be Accid::Undef if _first is Accid::Undef
    /// otherwise, must be larger than _first.
    enum Accid _second;
    
};

std::ostream& operator<<(std::ostream& o, const AccidPair& p);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::AccidPair> : fmt::ostream_formatter {};

#endif /* AccidPair_hpp */

/// @}
