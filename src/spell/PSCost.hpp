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
    bool operator>(const PSCost& rhs) const;

    /// access cumulated number of accidents.
    inline int getAccid() const { return _accid; }
    
    /// modify cumulated number of accidents.
    inline void incrAccid(int a=1) { _accid += a; }
    
    /// access cumulated distance to tonic.
    inline int getDist() const { return _dist; }

    /// modify cumulated distance to tonic.
    inline void incrDist(int d=1) { _dist += d; }

    /// cumulated number of non-conjoint moves.
    inline int getDisj() const { return _disj; }

    /// modify number of non-conjoint moves.
    inline void incrDisj(int d=1) { _disj += d; }
    
    /// cumulated number of accidentals with color different from global ton.
    inline int getColor() const { return _color; }

    /// modify number of accidentals with color different from global ton.
    inline void incrColor(int d=1) { _color += d; }
    
    void print(std::ostream& o) const;

private:
    
    /// cumulated number of accidents in the minimal path to this config.
    unsigned int _accid;
    
    /// cumulated distance to a conjectured local tonality
    /// in the minimal path to this config.
    unsigned int _dist;

    /// cumulated number of non-conjoint moves
    /// in the minimal path to this config.
    unsigned int _disj;

    /// cumulated number of accidentals with color different from global ton
    /// in the minimal path to this config.
    unsigned int _color;

};


std::ostream& operator<<(std::ostream& o, const PSCost& c);


} // namespace pse

#endif /* PSCost_hpp */

/// @}
