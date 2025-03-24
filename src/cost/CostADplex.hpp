//
//  CostADplex.hpp
//  pse
//
//  Created by Florent Jacquemard on 24/02/2025.
//
/// @addtogroup pitch
/// @{


#ifndef CostADplex_hpp
#define CostADplex_hpp

#include <stdio.h>
#include <assert.h>
#include <memory>

#include "pstrace.hpp"
#include "Cost.hpp"
#include "CostADplus.hpp"


namespace pse {

/// variant of CostAD
/// where the number of accidents and distance to local ton are summed
/// before comparison.
class CostADplex : public CostADplus
{
public: // construction
    
    /// null cost.
    /// @param tb_lex use lexicographic comparison for tie-breaking components.
    /// Otherwise make the sum of some tie-breaking components before
    /// comparison.
    CostADplex(bool tb_lex=true);
    
    /// copy constructor.
    CostADplex(const CostADplex& rhs);
    
    /// distructor
    virtual ~CostADplex();

    /// create a new null cost value.
    std::shared_ptr<Cost> shared_zero() const override
    { return Cost::shared_zero<CostADplex>(this->_tblex); }
       
    /// create a shared clone of this cost.
    std::shared_ptr<Cost> shared_clone() const override
    { return Cost::shared_clone<CostADplex>(); }
    
    /// create a smart clone of this cost.
    virtual std::unique_ptr<Cost> unique_clone() const
    { return Cost::unique_clone<CostADplex>(); }
    
public: // operators called in Cost

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const CostADplex& rhs) const;
       
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const CostADplex& rhs) const;
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    Cost& add(const CostADplex& rhs);

    /// a distance value, in percent of the bigger cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const CostADplex& rhs) const;

protected: // operators

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const Cost& rhs) const override
    { return Cost::equal<CostADplex>(rhs); }
        
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const Cost& rhs) const override
    { return Cost::smaller<CostADplex>(rhs); }
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    Cost& add(const Cost& rhs) override
    { return Cost::add<CostADplex>(rhs); }
    
    /// a distance value, in percent of the smaller cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const Cost& rhs) const override
    { return Cost::pdist<CostADplex>(rhs); }
           
public: // debug
    
    /// Cost type of this const value.
    virtual CostType type() const override;
    
};
     
std::ostream& operator<<(std::ostream& o, const CostADplex& c);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostADplex> : fmt::ostream_formatter {};

#endif /* CostADplex_hpp */

/// @}
