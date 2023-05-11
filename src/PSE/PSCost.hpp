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
    PSCost();   // const CostOrdering& co = CostOrdering::Default
    
    /// copy constructor
    PSCost(const PSCost& c);
    
    virtual ~PSCost();
    
    /// assignement operator
    PSCost& operator=(const PSCost& rhs);
    
    /// sum operator (non const).
    /// update this cost adding rhs.
    /// @param rhs a cost to add.
    /// @warning the ordering of this cost is preserved,
    /// the ordering of rhs is ignored.
    PSCost& operator+=(const PSCost& rhs);

    /// sum operator (const).
    /// @param rhs a cost to add.
    /// @return a copy of this cost to which rhs is added.
    /// @warning the ordering of the result is the ordering
    /// of this cost, the ordering of rhs is ignored.
    PSCost operator+(const PSCost& rhs) const;
    
    // default cost equality, according to the ordering code of this cost.
    // @warning the ordering of rhs is ignored.
    /// default cost equality.
    /// @warning static choice (at compile time).
    inline bool operator==(const PSCost& rhs) const
    { return eq_lex(rhs); }
    //{ return eq_cumul(rhs); }
    
    /// negation of ==.
    bool operator!=(const PSCost& rhs) const;

    // default ordering for costs, according to the ordering code of this cost.
    // @warning the ordering of rhs is ignored.
    /// default ordering for costs.
    /// @warning static choice (at compile time).
    inline bool operator<(const PSCost& rhs) const
    { return less_lex(rhs); }
    //{ return less_cumul(rhs); }

    /// negation of >.
    bool operator<=(const PSCost& rhs) const;

    /// inverse (commutation) of <.
    bool operator>(const PSCost& rhs) const;

    /// negation of <.
    bool operator>=(const PSCost& rhs) const;
    
    
    /// same cost components (exactly).
    bool eq_lex(const PSCost& rhs) const;

    /// negation of eq_lex.
    bool neq_lex(const PSCost& rhs) const;

    /// lexicographic ordering for costs.
    bool less_lex(const PSCost& rhs) const;

    /// negation of greater_lex.
    bool leq_lex(const PSCost& rhs) const;

    /// inverse (commutation) of less_lex.
    bool greater_lex(const PSCost& rhs) const;

    /// negation of less_lex.
    bool geq_lex(const PSCost& rhs) const;
    
     
    /// equality with cumul of number of accid
    /// and non-diatonic moves.
    bool eq_cumul(const PSCost& rhs) const;

    /// negation of eq_cumul
    bool neq_cumul(const PSCost& rhs) const;

    /// lexicographhic with with cumul of number of accid
    /// and non-diatonic moves.
    bool less_cumul(const PSCost& rhs) const;

    /// negation of greater_cumul.
    bool leq_cumul(const PSCost& rhs) const;

    /// inverse (commutation) of less_cumul.
    bool greater_cumul(const PSCost& rhs) const;

    /// negation of less_cumul.
    bool geq_cumul(const PSCost& rhs) const;

    
    /// degree of approximation.
    /// percent under which 2 costs componnents are considered equal.
    static double getApproxDegree();

    /// degree of approximation.
    /// percent under which 2 costs componnents are considered equal.
    static void setApproxDegree(double d);

    /// diff between a1 and a2 over base is less than approx degree.
    static bool approxeq(size_t a1, size_t a2, size_t base);
    
    /// equality with approximate same number of accid.
    /// @warning experimental
    bool eq_approx(const PSCost& rhs, size_t base) const;

    /// negation of eq_approx.
    bool neq_approx(const PSCost& rhs, size_t base) const;

    /// lexicographic with approximate same number of accid.
    /// @warning experimental
    bool less_approx(const PSCost& rhs, size_t base) const;

    /// negation of greater_approx.
    bool leq_approx(const PSCost& rhs, size_t base) const;

    /// inverse (commutation) of less_approx.
    bool greater_approx(const PSCost& rhs, size_t base) const;

    /// negation of less_approx.
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
    /// @param gton conjectured main (global) tonality (key signature).
    void update(const PSC1& c, const PSEnum& e, const Ton& gton);

    /// update this cost for doing a transition into the given config.
    /// the note read for the transition is the id of the config minus one.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    void update(const PSC1& c, const PSEnum& e,
                const Ton& gton, const Ton& lton);
    
    /// update this cost for doing a transition into the given chord config.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the acciental must be printed in score.
    /// @param nbocc nb of occurrence of the note (pitch class) in this chord.
    /// @param gton conjectured main (global) tonality (key signature).
    void update(const PSC2& c, const PSEnum& e,
                const enum NoteName& name, const enum Accid& accid,
                bool print, size_t nbocc,
                const Ton& gton);
    
    /// update this cost for doing a transition into the given chord config.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the acciental must be printed in score.
    /// @param nbocc nb of occurrence of the note (pitch class) in this chord.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    void update(const PSC2& c, const PSEnum& e,
                const enum NoteName& name, const enum Accid& accid,
                bool print, size_t nbocc,
                const Ton& gton, const Ton& lton);

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
    
    /// horizontal cost
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
    
    //CostOrdering _order;
    
    /// degree of approximation.
    /// percent under which 2 costs componnents are considered equal.
    static double approx_degree;

    // internal constructor.
    // PSCost(size_t a, size_t n, size_t d, size_t c);
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
