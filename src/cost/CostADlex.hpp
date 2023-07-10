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

#include "trace.hpp"
#include "Cost.hpp"
#include "Costt.hpp"
#include "CostAD.hpp"



namespace pse {

class CostADlex : public CostAD, // update
                  public PolymorphicComparable<Cost, CostADlex>
{
public:
    
    /// null cost.
    CostADlex();
    
    /// copy constructor.
    CostADlex(const CostADlex& rhs);
    
    /// distructor
    virtual ~CostADlex();
    
    /// cost equality.
    /// @param rhs a cost to compare to.
    bool operator==(const CostADlex& rhs) const;
    
    /// a distance value, in percent of the bigger cost.
    /// used for approximate equality.
    double dist(const CostADlex& rhs) const;
    
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool operator<(const CostADlex& rhs) const;
        
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    CostADlex& operator+=(const CostADlex& rhs);

    /// create a new null cost value.
    virtual std::shared_ptr<Cost> shared_zero() const override;
    
    /// create a shared clone of this cost.
    virtual std::shared_ptr<Cost> shared_clone() const override;
    
    // create a smart clone of this cost.
    // virtual std::unique_ptr<Cost> unique_clone() const override;
    
    /// update this cost for doing a transition renaming one note (single
    /// or in chord) with the given parameters and in a given hypothetic global
    /// local tonalities.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// not known.
    virtual void update(const enum NoteName& name,
                        const enum Accid& accid,
                        bool print,
                        const Ton& gton, const Ton& lton = Ton()) override;
    
    /// @param o output stream where to print this cost.
    void print(std::ostream& o) const override;

};
    
    
std::ostream& operator<<(std::ostream& o, const CostADlex& c);


} // namespace pse

#endif /* CostADlex_hpp */

/// @}

