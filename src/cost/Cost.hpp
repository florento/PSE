//
//  Cost.hpp
//  pse
//
//  Created by Florent on 16/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef Cost_hpp
#define Cost_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Ton.hpp"

namespace pse {

class PSC1;

class PSC2;


/// abstract class defining the cost update interface.
class Cost
{
public:

    /// create a new null cost value.
    virtual std::shared_ptr<Cost> shared_zero() const = 0;

    /// create a shared clone of this cost.
    virtual std::shared_ptr<Cost> shared_clone() const = 0;
    virtual std::unique_ptr<Cost> unique_clone() const = 0;

    /// equality (mandatory).
    /// @param rhs another cost to compare to.
    virtual bool operator==(const Cost& rhs) const = 0;

    /// negation of equality.
    /// @param rhs another cost to compare to.
    virtual bool operator!=(const Cost& rhs) const;
    
    /// a distance value, in percent of the bigger cost.
    /// used for approximate equality.
    virtual double dist(const Cost& rhs) const = 0;

    /// strictly less (mandatory).
    /// @param rhs another cost to compare to.
    virtual bool operator<(const Cost& rhs) const = 0;

    /// negation of >.
    /// @param rhs another cost to compare to.
    virtual bool operator<=(const Cost& rhs) const;

    /// inverse (commutation) of <.
    /// @param rhs another cost to compare to.
    virtual bool operator>(const Cost& rhs) const;

    /// negation of <.
    /// @param rhs another cost to compare to.
    virtual bool operator>=(const Cost& rhs) const;
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    virtual Cost& operator+=(const Cost& rhs) = 0;
    
    /// update this cost for doing a transition into the given config,
    /// from its previous config, in a given hypothetic global tonality.
    /// @see PSC.previous()
    /// @param gton conjectured main (global) tonality (key signature).
    virtual void update(const PSC1& c, const PSEnum& e,
                        const Ton& gton) = 0;

    /// update this cost for doing a transition into the given config,
    /// from its previous config,
    /// in given hypothetic global and local tonalities.
    /// @see PSC.previous()
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    virtual void update(const PSC1& c, const PSEnum& e,
                        const Ton& gton, const Ton& lton) = 0;
    
    /// update this cost for doing a transition into the given chord config,
    /// in a given hypothetic global tonality.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the accidental must be printed in score.
    /// @param nbocc nb of occurrence of the note (pitch class) in this chord.
    /// @param gton conjectured main (global) tonality (key signature).
    virtual void update(const PSC2& c, const PSEnum& e,
                        const enum NoteName& name, const enum Accid& accid,
                        bool print, size_t nbocc,
                        const Ton& gton) = 0;
    
    /// update this cost for doing a transition into the given chord config,
    /// in given hypothetic global and local tonalities.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the accidental must be printed in score.
    /// @param nbocc nb of occurrence of the note (pitch class) in this chord.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    virtual void update(const PSC2& c, const PSEnum& e,
                        const enum NoteName& name, const enum Accid& accid,
                        bool print, size_t nbocc,
                        const Ton& gton, const Ton& lton) = 0;
    
    virtual void print(std::ostream& o) const = 0;
};


/// convenience function.
/// @param lhs first value. must be positive or null.
/// @param rhs second value. must be positive or null.
/// @return the difference between lhs amd rhs, in percent of the biggest one.
double distCost(const double lhs, const double rhs);



std::ostream& operator<<(std::ostream& o, const Cost& c);


} // namespace pse

#endif /* Cost_hpp */

/// @}
