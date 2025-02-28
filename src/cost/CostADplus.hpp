//
//  CostADplus.hpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//
/// @addtogroup pitch
/// @{

#ifndef CostADplus_hpp
#define CostADplus_hpp

#include <stdio.h>
#include <assert.h>
#include <memory>

#include "pstrace.hpp"
#include "Cost.hpp"
// #include "Costt.hpp"
#include "CostAD.hpp"


namespace pse {


/// variant of CostAD
/// where the number of accidents and distance to local ton are summed
/// (in accid field) before comparison.
class CostADplus : public CostAD
{   
public: // construction
    
    /// null cost.
    /// @param tb_sum make the sum of some tie-breaking components
    /// before comparison.
    CostADplus(bool tb_sum=false);
    
    /// copy constructor.
    CostADplus(const CostADplus& rhs);
    
    /// distructor
    virtual ~CostADplus();

    /// create a new null cost value.
    std::shared_ptr<Cost> shared_zero() const override
    { return Cost::shared_zero<CostADplus>(this->_tblex); }
    
    /// create a shared clone of this cost.
    std::shared_ptr<Cost> shared_clone() const override
    { return Cost::shared_clone<CostADplus>(); }
    
    /// create a smart clone of this cost.
    std::unique_ptr<Cost> unique_clone() const
    { return Cost::unique_clone<CostADplus>(); }

public: // operators called in Cost

    /// cost equality.
    /// @param rhs a cost to compare to.
    /// @see same as CostAT: accid and dist summed in accid, dist ignored.
    bool equal(const CostADplus& rhs) const;
    
    /// cost inequality.
    /// @param rhs a cost to compare to.
    /// @see same as CostAT: accid and dist summed in accid, dist ignored.
    bool smaller(const CostADplus& rhs) const;
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    /// @see same as CostAD
    Cost& add(const CostADplus& rhs);

    /// a distance value, in percent of the bigger cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const CostADplus& rhs) const;

protected: // operators

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const Cost& rhs) const override
    { return Cost::equal<CostADplus>(rhs); }
        
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const Cost& rhs) const override
    { return Cost::smaller<CostADplus>(rhs); }
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    Cost& add(const Cost& rhs) override
    { return Cost::add<CostADplus>(rhs); }
    
    /// a distance value, in percent of the smaller cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const Cost& rhs) const override
    { return Cost::pdist<CostADplus>(rhs); }
        
public: // update
    
    /// update this cost for doing a transition renaming one note (single
    /// or in chord) with the given parameters and in a given hypothetic global
    /// local tonalities.
    /// @param name chosen name for the received pitch,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// not known.
    /// @param prev_name previous name associated to has been associated to
    /// the received pitch (before processing it). Notename::Undef if the pitch
    /// was never assiated a name in the configuration's state.
    /// @return wether an update was effectively performed.
    /// @see perform the update of CostAD and add dist to accid.
    bool update(const enum NoteName& name,
                const enum Accid& accid,
                bool print,
                const Ton& gton, const Ton& lton = Ton(),
                const enum NoteName& prev_name = NoteName::Undef) override;

protected: // access
    
    /// real number of accids.
    /// in this representation,
    /// _accids is the sum of the number accids and dist.
    size_t accids() const;
        
public: // debug
    
    /// Cost type of this const value.
    virtual CostType type() const override;
    
    /// @param o output stream where to print this cost.
    void print(std::ostream& o) const override;
    
protected: // data
    
    /// _accid contains the sum of printed accidentals and distance
    
    // sum of number of printed accidentals and
    // distance to a conjectured local tonality.
    // update: the sum is stored in _accid
    // size_t _sum;
    
};
     
std::ostream& operator<<(std::ostream& o, const CostADplus& c);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostADplus> : fmt::ostream_formatter {};

#endif /* CostADplus_hpp */

/// @}

