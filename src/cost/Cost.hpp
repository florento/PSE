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

#include "pstrace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "Accids.hpp"
#include "Ton.hpp"
#include "CostType.hpp"


namespace pse {


/// Cost model for the ordering of configuration of the PS algorithm.
/// @see Note Spelling Conventions in Behind Bars (page 85).
/// this abstract class defines the cost update interface.
class Cost
{
    
public: // construction
    
    /// create a new null cost value.
    virtual std::shared_ptr<Cost> shared_zero() const = 0;
    
    /// create a shared clone of this cost.
    virtual std::shared_ptr<Cost> shared_clone() const = 0;
    
    // create a unique clone of this cost.
    // virtual std::unique_ptr<Cost> unique_clone() const = 0;

    virtual Cost& operator=(const Cost& rhs) = delete;
    
protected: // construction templates

    /// create a new null cost value.
    template<typename T>
    std::shared_ptr<Cost> shared_zero() const;

    /// create a new null cost value, with flag.
    template<typename T>
    std::shared_ptr<Cost> shared_zero(bool flag) const;

    /// create a shared clone of this cost.
    template<typename T>
    std::shared_ptr<Cost> shared_clone() const;
        
    /// create a unique clone of this cost.
    template<typename T>
    std::unique_ptr<Cost> unique_clone() const;

public: // operators, update

    /// equality (mandatory).
    /// @param rhs another cost to compare to.
    bool operator==(const Cost& rhs) const;
    
    /// negation of equality.
    /// @param rhs another cost to compare to.
    bool operator!=(const Cost& rhs) const;
    
    /// a distance value, in percent of the bigger cost.
    /// used for approximate equality.
    double dist(const Cost& rhs) const;
    
    /// strictly less (mandatory).
    /// @param rhs another cost to compare to.
    bool operator<(const Cost& rhs) const;
    
    /// negation of >.
    /// @param rhs another cost to compare to.
    bool operator<=(const Cost& rhs) const;
    
    /// inverse (commutation) of <.
    /// @param rhs another cost to compare to.
    bool operator>(const Cost& rhs) const;
    
    /// negation of <.
    /// @param rhs another cost to compare to.
    bool operator>=(const Cost& rhs) const;
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    Cost& operator+=(const Cost& rhs);

protected: // comparison operators
    
    /// cost equality.
    /// @param rhs another cost to compare to.
    virtual bool equal(const Cost& rhs) const = 0;

    /// strict inequality of costs.
    /// @param rhs another cost to compare to.
    virtual bool smaller(const Cost& rhs) const = 0;

    /// cumulated sum of costs. update this cost by adding rhs.
    /// @param rhs a cost to add.
    virtual Cost& add(const Cost& rhs) = 0;

    /// a distance value, in percent of the smallest cost between this and rhs.
    /// @return 0 if this and rhs are not comparable for this measure,
    /// a negative value (percent) is this is larger to rhs,
    /// a positive value (percent) is this is smaller to rhs.
    /// @warning only used for selection of global
    /// in TonIndex (rowcost comparison) and Gridy computation.
    virtual double pdist(const Cost& rhs) const = 0;

protected: // comparison operators templates and static

    /// cost equality.
    /// @param rhs another cost to compare to.
    /// @see used by ==
    template<typename T>
    bool equal(const Cost& rhs) const;

    /// strict inequality of costs.
    /// @param rhs another cost to compare to.
    /// @see used by <
    template<typename T>
    bool smaller(const Cost& rhs) const;

    /// cumulated sum of costs. update this cost by adding rhs.
    /// @param rhs a cost to add.
    /// @see used by +=
    template<typename T>
    Cost& add(const Cost& rhs);

    /// a distance value, in percent of the smallest cost between this and rhs.
    /// @return 0 if this and rhs are not comparable for this measure,
    /// a negative value (percent) is this is larger to rhs,
    /// a positive value (percent) is this is smaller to rhs.
    /// @warning only used for selection of global
    /// in TonIndex (rowcost comparison) and Gridy computation.
    template<typename T>
    double pdist(const Cost& rhs) const;
        
    /// the difference between lhs amd rhs, in percent of the smaller one.
    /// @param lhs first value. must be positive or null.
    /// @param rhs second value. must be positive or null.
    /// @return the difference between lhs amd rhs,
    /// in percent of the smaller one.
    /// @return 0 if lhs and rhs are equal.
    /// a negative value (percent of diff) if lhs is smaller than rhs,
    /// a positive value (percent of diff) if lhs is larger than rhs.
    static double dist(const double lhs, const double rhs);
    
public: // operators, update
    
    /// update this cost for doing a transition renaming one note (single
    /// or in chord) with the given parameters and in a given hypothetic global
    /// local tonalities.
    /// @param name chosen name for the received pitch,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param printed whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality or undef tonality by default.
    /// not known.
    /// @param prev_name previous name associated to has been associated to
    /// the received pitch (before processing it). Notename::Undef if the pitch
    /// was never assiated a name in the configuration's state.
    /// @return wether an update was effectively performed.
    virtual bool update(const enum NoteName& name,
                        const enum Accid& accid,
                        bool printed,
                        const Ton& gton,
                        const Ton& lton = Ton(),
                        const enum NoteName& prev_name = NoteName::Undef) = 0;
    
public: // access and debug
    
    /// Cost type of this cost value.
    virtual CostType type() const = 0;
    
    virtual void print(std::ostream& o) const;
    
};

std::ostream& operator<<(std::ostream& o, const Cost& c);

} // namespace pse

// separated definition of template methods
#include "Cost.tpp"


/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::Cost> : fmt::ostream_formatter {};

#endif /* Cost_hpp */

/// @}



// class PSC1;
// class PSC2;

// update this cost for doing a transition into the given config,
// from its previous config, in a given hypothetic global tonality.
// @see PSC.previous()
// @param gton conjectured main (global) tonality (key signature).
// virtual void update(const PSC1& c, const PSEnum& e,
//                     const Ton& gton) = 0;

// update this cost for doing a transition into the given config,
// from its previous config,
// in given hypothetic global and local tonalities.
// @see PSC.previous()
// @param gton conjectured main (global) tonality (key signature).
// @param lton conjectured local tonality.
// virtual void update(const PSC1& c, const PSEnum& e,
//                     const Ton& gton, const Ton& lton = Ton()) = 0;

// update this cost for doing a transition into the given chord config,
// in a given hypothetic global tonality.
// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
// @param accid chosen alteration for the received pitch, in -2..2.
// @param print whether the accidental must be printed in score.
// @param nbocc nb of occurrence of the note (pitch class) in this chord.
// @param gton conjectured main (global) tonality (key signature).
// virtual void update(const PSC2& c, const PSEnum& e,
//                     const enum NoteName& name, const enum Accid& accid,
//                     bool print, size_t nbocc,
//                     const Ton& gton) = 0;

// update this cost for doing a transition into the given chord config,
// in given hypothetic global and local tonalities.
// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
// @param accid chosen alteration for the received pitch, in -2..2.
// @param print whether the accidental must be printed in score.
// @param nbocc nb of occurrence of the note (pitch class) in this chord.
// @param gton conjectured main (global) tonality (key signature).
// @param lton conjectured local tonality.
// virtual void update(const PSC2& c, const PSEnum& e,
//                     const enum NoteName& name, const enum Accid& accid,
//                     bool print, size_t nbocc,
//                     const Ton& gton, const Ton& lton) = 0;
