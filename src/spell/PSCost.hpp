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
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Ton.hpp"

namespace pse {


class PSC1;
class PSC2;


/// Cost model for the ordering of configuration of the PS algorithm.
// see Note Spelling Conventions in Behind Bars (page 85)
struct PSCost
{
public:

    /// null cost (neutral element for plus).
    PSCost();
    
    /// copy constructor
    PSCost(const PSCost& c);
    
    virtual ~PSCost();
    
    /// assignement operator
    PSCost& operator=(const PSCost& rhs);
    
    /// sum operator (non const).
    PSCost& operator+=(const PSCost& rhs);

    /// sum operator (const).
    PSCost operator+(const PSCost& rhs) const;
    
    /// same cost componnents (exactly)
    bool operator==(const PSCost& rhs) const;
    
    /// different costs
    bool operator!=(const PSCost& rhs) const;

    /// lexicographic ordering for costs.
    /// default ordering.
    bool operator<(const PSCost& rhs) const;

    /// lexicographic ordering for costs
    bool operator<=(const PSCost& rhs) const;

    /// lexicographic ordering for costs
    bool operator>(const PSCost& rhs) const;

    /// lexicographic ordering for costs
    bool operator>=(const PSCost& rhs) const;
    
     
    /// equality with cumul of number of accid
    /// and non-diatoinc moves.
    bool eq_cumul(const PSCost& rhs) const;

    /// negation of eq_cumul
    bool neq_cumul(const PSCost& rhs) const;

    /// lexicographhic with with cumul of number of accid
    /// and non-diatoinc moves.
    bool less_cumul(const PSCost& rhs) const;

    bool leq_cumul(const PSCost& rhs) const;

    bool greater_cumul(const PSCost& rhs) const;

    bool geq_cumul(const PSCost& rhs) const;
       
    
    /// equality with approximate same nunmber of accid.
    /// @warning experimental
    bool eq_approx(const PSCost& rhs, size_t base) const;

    /// negation of eq_approx
    bool neq_approx(const PSCost& rhs, size_t base) const;

    /// lexicographhic with approximate same nunmber of accid.
    /// @warning experimental
    bool less_approx(const PSCost& rhs, size_t base) const;

    bool leq_approx(const PSCost& rhs, size_t base) const;

    bool greater_approx(const PSCost& rhs, size_t base) const;

    bool geq_approx(const PSCost& rhs, size_t base) const;
    
    /// access cumulated number of accidents.
    inline size_t getAccid() const { return _accid; }
    
    /// cumulated number of non-conjoint moves.
    inline size_t getDia() const { return _ndia; }

    /// access cumulated distance to tonic.
    inline size_t getDist() const { return _dist; }

    /// cumulated number of accidentals with color different from global ton.
    inline size_t getColor() const { return _color; }

    /// update this cost for doing a transition into the given config.
    /// the note read for the transition is the id of the config minus one.
    void update(const PSC1& c, const PSEnum& e, const Ton& ton);

    /// update this cost for doing a transition into the given config.
    /// the note read for the transition is the id of the config minus one.
    void update(const PSC1& c, const PSEnum& e,
                const Ton& ton, const Ton& lton);

    /// modify cumulated number of accidents.
    inline void incrAccid(size_t a=1) { _accid += a; }
    
    /// modify number of non-conjoint moves.
    inline void incrDia(size_t d=1) { _ndia += d; }

    /// modify cumulated distance to tonic.
    inline void incrDist(size_t d=1) { _dist += d; }
    
    /// modify number of accidentals with color different from global ton.
    inline void incrColor(size_t d=1) { _color += d; }
    
    void print(std::ostream& o) const;

private:
    
    /// cumulated number of printed accidentals
    /// in the minimal path to this config.
    size_t _accid; // unsigned int
    
    /// cumulated number of non-diatonic moves
    /// in the minimal path to this config.
    /// conjoint move = adjacent pitch letters (stepwise figures as a scale)
    // replace by augmented and diminished intervals ?
    size_t _ndia;

    /// cumulated distance to a conjectured local tonality
    /// in the minimal path to this config.
    size_t _dist;

    /// cumulated number of accidentals with color different from global ton
    /// in the minimal path to this config.
    size_t _color;
    
    // internal constructor.
    // PSCost(size_t a, size_t n, size_t d, size_t c);
    
    static bool approxeq(size_t a1, size_t a2, size_t base);
    
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


PSCost operator+(const PSCost& c1, const PSCost& c2);

std::ostream& operator<<(std::ostream& o, const PSCost& c);


} // namespace pse

#endif /* PSCost_hpp */

/// @}
