//
//  CostADlex.hpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//
/// @addtogroup pitch
/// @{


#ifndef CostADlex_hpp
#define CostADlex_hpp

#include <stdio.h>
#include <assert.h>
#include <memory>

#include "pstrace.hpp"
#include "Cost.hpp"
// #include "Costt.hpp"
#include "CostAD.hpp"



namespace pse {

/// variant of CostAD
/// where the number of accidents and distance to local ton are compared
/// lexicographically.
class CostADlex : public CostAD // public PolymorphicCost<CostADlex>
{
    
public: // construction
    
    /// null cost.
    /// @param tb_lex use lexicographic comparison for tie-breaking components.
    /// Otherwise make the sum of some tie-breaking components before
    /// comparison.
    CostADlex(bool tb_lex=true);
    
    /// copy constructor.
    CostADlex(const CostADlex& rhs);
    
    /// distructor
    virtual ~CostADlex();

    /// create a new null cost value.
    std::shared_ptr<Cost> shared_zero() const override
    { return Cost::shared_zero<CostADlex>(this->_tblex); }
    
    /// create a shared clone of this cost.
    std::shared_ptr<Cost> shared_clone() const override
    { return Cost::shared_clone<CostADlex>(); }
    
    /// create a smart clone of this cost.
    std::unique_ptr<Cost> unique_clone() const
    { return Cost::unique_clone<CostADlex>(); }
    
public: // operators called in Cost
    
    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const CostADlex& rhs) const;
    
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const CostADlex& rhs) const;
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    /// @see same as CostAD
    Cost& add(const CostADlex& rhs);

    /// a distance value, in percent of the bigger cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const CostADlex& rhs) const;
    
protected: // operators

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const Cost& rhs) const override
    { return Cost::equal<CostADlex>(rhs); }
        
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const Cost& rhs) const override
    { return Cost::smaller<CostADlex>(rhs); }
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    Cost& add(const Cost& rhs) override
    { return Cost::add<CostADlex>(rhs); }
    
    /// a distance value, in percent of the smaller cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const Cost& rhs) const override
    { return Cost::pdist<CostADlex>(rhs); }

public: // access, debug
    
    /// Cost type of this const value.
    virtual CostType type() const override;
    
    // @param o output stream where to print this cost.
    // @see same as CostAD::print
    // void print(std::ostream& o) const override;

};
        
std::ostream& operator<<(std::ostream& o, const CostADlex& c);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostADlex> : fmt::ostream_formatter {};

#endif /* CostADlex_hpp */

/// @}

