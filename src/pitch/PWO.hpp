//
//  PWO.hpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PWO_hpp
#define PWO_hpp

#include <iostream>

#include "NoteName.hpp"
#include "Accidental.hpp"

namespace pse {


/// Pitch without octave
class PWO
{
public:
    
    PWO();
    
    /// main constructor
    PWO(const enum NoteName& n, const enum Accid& a);
    
    /// @brief copy constructor.
    PWO(const PWO& p);
    
    /// destructor
    virtual ~PWO() {}
    
    /// assignment operator
    virtual PWO& operator=(const PWO&);

    /// equality
    virtual bool equal(const PWO&) const;

    /// unpitched
    virtual bool undef() const;

    /// unpitched
    /// @see undef()
    virtual bool unpitched() const { return undef(); }

    virtual void print(std::ostream& o) const;
    
public: // data
    
    /// @brief note name betwen 'A' and 'G'.
    /// @see MusicXML step https://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-step.htm
    /// @todo revise with NoteName
    enum NoteName name;

    /// @brief alteration in [-2, 2] where 1.0 is half tone.
    /// e.g. -1 for flat, 1 for sharp
    /// decimal values are used for microtones
    /// e.g. 0.5 for quarter tone sharp
    /// @see MusicXML alter https://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-alter.htm
    /// revision: @see https://www.w3.org/2021/06/musicxml40/musicxml-reference/data-types/accidental-value/
    enum Accid alteration;
    
};

// boolean operators
bool operator==(const PWO& lhs, const PWO& rhs);

bool operator!=(const PWO& lhs, const PWO& rhs);

std::ostream& operator<<(std::ostream& o, const PWO& p);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::PWO> : fmt::ostream_formatter {};

#endif /* PWO_hpp */

/// @}
