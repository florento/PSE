//
//  IntervalSimple.hpp
//  pse
//
//  Created by Florent Jacquemard on 13/02/2023.
//
/// @addtogroup pitch
/// @{

#ifndef IntervalSimple_hpp
#define IntervalSimple_hpp

#include <iostream>
#include <assert.h>
#include <string>
#include <stdlib.h>     //  abs
#include <vector>

#include "trace.hpp"
#include "Pitch.hpp"

namespace pse {

/// simple intervals (smaller than or equal to an octave)
/// belonging to the following cases.
///
/// |1/2t| quality       | number | abbr     |   |
/// |----|---------------|--------|----------|---|
/// | 0  | Perfect       | Unison | P1       | 0 |
/// | 0  | Diminished    | 2nd    | D2       | 2 |
/// | 1  | Augmented     | Unison | A1       | 2 |
/// | 1  | Minor         | 2nd    | m2       | 0 |
/// | 1  | SubDiminished | 3rd    |          | 2 |
/// | 2  | OverAugmented | Unison |          | 2 |
/// | 2  | Major         | 2nd    | M2       | 0 |
/// | 2  | Diminished    | 3rd    | D3       | 2 |
/// | 3  | Augmented     | 2nd    | A2       | 1 |
/// | 3  | Minor         | 3rd    | m3       | 0 |
/// | 3  | SubDiminished | 4th    |          | 2 |
/// | 4  | OverAugmented | 2nd    |          | 2 |
/// | 4  | Major         | 3rd    | M3       | 0 |
/// | 4  | Diminished    | 4th    | D4       | 1 |
/// | 5  | Augmented     | 3rd    | A3       | 2 |
/// | 5  | Perfect       | 4th    | P4       | 0 |
/// | 5  | SubDiminished | 5th    |          | 2 |
/// | 6  | OverAugmented | 3rd    |          | 2 |
/// | 6  | Augmented     | 4th    | A4       | 1 |
/// | 6  | Diminished    | 5th    | D5       | 1 |
/// | 6  | SubDiminished | 6th    |          | 2 |
/// | 7  | OverAugmented | 4th    |          | 2 |
/// | 7  | Perfect       | 5th    | P5       | 0 |
/// | 7  | Diminished    | 6th    | D6       | 2 |
/// | 8  | Augmented     | 5th    | A5       | 1 |
/// | 8  | Minor         | 6th    | m6       | 0 |
/// | 8  | SubDiminished | 7th    |          | 2 |
/// | 9  | OverAugmented | 5th    |          | 2 |
/// | 9  | Major         | 6th    | M6       | 0 |
/// | 9  | Diminished    | 7th    | D7       | 1 |
/// | 10 | Augmented     | 6th    | A6       | 2 |
/// | 10 | Minor         | 7th    | m7       | 0 |
/// | 10 | SubDiminished | 8ve    |          | 2 |
/// | 11 | OverAugmented | 6th    |          | 2 |
/// | 11 | Major         | 7th    | M7       | 0 |
/// | 11 | Diminished    | 8ve    | D8       | 2 |
/// | 12 | Augmented     | 7th    | A7       | 2 |
/// | 12 | Perfect       | 8ve    | P8       | 0 |
/// | 13 | OverAugmented | 7th    |          | 2 |
/// |                                    
///
/// the last column in an interval category (interval cost, or rarety):
/// - 0: intervals in the natural diatonic scale (all minor, Major, Perfect)
/// - 1: intervals in the minor harmonic scale
///      A2 and renv. D7
///      A4 and D5 (triton)
///      D4 and A5
/// - 2: others (rare accidentals)
struct SimpleInterval
{
public:
    
    /// undefined interval name
    static const unsigned int UNDEF_INTERVAL;
    
    /// undefined interval cost
    static const unsigned int UNDEF_COST;
    
    /// Interval qualities
    /// @todo SubDiminished, OverAugmented are not standard?
    /// @see Fokker/Keenan Extended-diatonic interval-names (and 2016 revision)
    /// https://dkeenan.com/Music/IntervalNaming.htm
    enum class Quality
    {
        //SuperDiminished, // not used
        SubDiminished,
        Diminished,
        Minor,
        Perfect,
        Major,
        Augmented,
        OverAugmented,
        //SuperAugmented, // not used
        Undef
    };
    
    /// undefined interval
    SimpleInterval();
    
    /// accessor to interval number
    inline unsigned int number() const { return _nb; }
    
    /// accessor to interval quality
    inline Quality quality() const { return _quality; }

    /// accessor to interval cost (rarity)
    inline unsigned int cost() const { return _cost; }

    /// interval string representation
    static std::string tostring(const Quality& m);
    
    /// interval printer
    void print(std::ostream& o) const;
    
protected:
    
    /// interval number, between 1 (unison) and 8 (octave) or 0 (undef)
    unsigned int _nb;
    
    /// interval quality
    Quality _quality;
    
    /// interval category:
    /// - 0: the intervals in the natural diatonic scale (minor, major, perfect)
    /// - 2: rare accidentals
    /// - 1: others
    unsigned int _cost;
    
    /// main constructor.
    /// @param i interval number, between 1 (unison) and 8 (octave)
    /// @param q quality of the interval. must not be Quality::Undef
    SimpleInterval(unsigned int i, const Quality& q, unsigned int cost = 0);
    
    /// all interval, ranked by number of halftones
    static const std::vector<SimpleInterval> INTERVALS[15];
    
private:
    
    // abbreviations for interval quality
    // static const Quality _s2D;
    // static const Quality _sD;
    // static const Quality _D;
    // static const Quality _m;
    // static const Quality _P;
    // static const Quality _M;
    // static const Quality _A;
    // static const Quality _oA;
    // static const Quality _s2A;
    // static const Quality _U;
};

std::ostream& operator<<(std::ostream& o, const SimpleInterval::Quality& q);

std::ostream& operator<<(std::ostream& o, const SimpleInterval& i);

} // namespace pse

#endif /* IntervalSimple_hpp */

/// @}
