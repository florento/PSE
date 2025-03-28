//
//  IntervalHarmonic.hpp
//  pse
//
//  Created by Florent Jacquemard on 12/02/2023.
//
/// @addtogroup pitch
/// @{

#ifndef IntervalHarmonic_hpp
#define IntervalHarmonic_hpp

#include <iostream>
#include <assert.h>
#include <string>
#include <stdlib.h>     //  abs
#include <vector>

#include "pstrace.hpp"
#include "Pitch.hpp"
#include "IntervalSimple.hpp"


namespace pse {


/// 
class HarmonicInterval : public SimpleInterval
{
public:
    
    /// main constructor
    HarmonicInterval(const Pitch& p1, const Pitch& p2);
       
    /// interval printer
    void print(std::ostream& o) const;
    
private: // data
    
    /// interval is ascending or descending
    bool _ascending;
    
    /// number of semitons in 0..14
    unsigned int _dist;
        
    /// number of octaves in composed interval
    /// (0 for simple interval).
    unsigned int _octaves;
        
private:
    
    /// number of semitones between two pitches
    unsigned int MidiDist(const Pitch& p1, const Pitch& p2);
    
    /// constructor for name attribute
    void init_name(const Pitch& p1, const Pitch& p2);
    
    /// constructor for quality attribute
    void init_quality();

    // all interval, by number of halftones
    // static const std::vector<std::pair<unsigned int, Quality>> INTERVALS[14];
    
};

std::ostream& operator<<(std::ostream& o, const HarmonicInterval& i);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::HarmonicInterval> : 
                                                    fmt::ostream_formatter {};

#endif /* IntervalHarmonic_hpp */

/// @}
