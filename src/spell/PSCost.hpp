//
//  PSCost.hpp
//  pse
//
//  Created by Florent Jacquemard on 11/12/2022.
//
/// @addtogroup pitch
/// @{

#ifndef PSCost_hpp
#define PSCost_hpp

#include <iostream>
#include <assert.h>

#include "trace.hpp"


namespace pse {

/// Cost model for the ordering of configuration of the PS algorithm.
// see Note Spelling Conventions in Behind Bars (page 85)

struct PSCost
{
public:

    PSCost();
    
    /// copy constructor
    PSCost(const PSCost& c);
    
    virtual ~PSCost();
    
    /// assignement operator
    PSCost& operator=(const PSCost& rhs);
    
    /// same costs
    bool operator==(const PSCost& rhs) const;
    
    /// different costs
    bool operator!=(const PSCost& rhs) const;

    /// lexicographic ordering for costs
    bool operator<(const PSCost& rhs) const;

    /// lexicographic ordering for costs
    bool operator<=(const PSCost& rhs) const;

    /// lexicographic ordering for costs
    bool operator>(const PSCost& rhs) const;

    /// lexicographic ordering for costs
    bool operator>=(const PSCost& rhs) const;

    /// access cumulated number of accidents.
    inline size_t getAccid() const { return _accid; }
    
    /// modify cumulated number of accidents.
    inline void incrAccid(size_t a=1) { _accid += a; }
    
    /// access cumulated distance to tonic.
    inline size_t getDist() const { return _dist; }

    /// modify cumulated distance to tonic.
    inline void incrDist(size_t d=1) { _dist += d; }

    /// cumulated number of non-conjoint moves.
    inline size_t getDisj() const { return _disj; }

    /// modify number of non-conjoint moves.
    inline void incrDisj(size_t d=1) { _disj += d; }
    
    /// cumulated number of accidentals with color different from global ton.
    inline size_t getColor() const { return _color; }

    /// modify number of accidentals with color different from global ton.
    inline void incrColor(size_t d=1) { _color += d; }
    
    void print(std::ostream& o) const;

private:
    
    /// cumulated number of printed accidentals
    /// in the minimal path to this config.
    size_t _accid; // unsigned int
    
    /// cumulated distance to a conjectured local tonality
    /// in the minimal path to this config.
    size_t _dist;

    /// cumulated number of non-conjoint moves
    /// in the minimal path to this config.
    /// conjoint move = adjacent pitch letters (stepwise figures as a scale)
    // replace by augmented and diminished intervals ?
    size_t _disj;

    /// cumulated number of accidentals with color different from global ton
    /// in the minimal path to this config.
    size_t _color;

};


// Note Spelling Conventions in Behind Bars (page 85)
// other rules
//
// - Use the most familiar intervals - perfect, minor and major
//   rather than augmented and diminished intervals.
//   melodic and harmonic intervals ?
//
// - Chromatic-scale figures use sharps to ascend, flats to descend
//
// - Spell stepwise figures as a scale, i.e. as adjacent pitch letters.
//   e.g. D#, E, Fb and not Eb, Enat, E#




std::ostream& operator<<(std::ostream& o, const PSCost& c);


} // namespace pse

#endif /* PSCost_hpp */

/// @}
