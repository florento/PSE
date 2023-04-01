//
//  Interval.hpp
//  pse
//
//  Created by Florent Jacquemard on 12/02/2023.
//
/// @addtogroup pitch
/// @{

#ifndef Interval_hpp
#define Interval_hpp

#include <iostream>
#include <assert.h>
#include <string>
#include <stdlib.h>     //  abs
#include <vector>

#include "trace.hpp"
#include "Pitch.hpp"
#include "IntervalSimple.hpp"


namespace pse {


/// 
class Interval : public SimpleInterval
{
public:
    
    /// main constructor
    Interval(const Pitch& p1, const Pitch& p2);
       
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


std::ostream& operator<<(std::ostream& o, const Interval& i);


} // namespace pse

#endif /* Interval_hpp */

/// @}
