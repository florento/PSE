//
//  CostA.hpp
//  pse
//
//  Created by Florent on 16/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef CostA_hpp
#define CostA_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "Cost.hpp"
#include "Costt.hpp"


namespace pse {

class PSC1;

class PSC2;

class CostA : public PolymorphicComparable<Cost, CostA>
{
    
public:
    
    /// null cost.
    CostA();
    
    /// copy constructor.
    CostA(const CostA& rhs);
    
    /// distructor
    virtual ~CostA();
    
    // assignement operator.
    // @param rhs a cost to copy.
    // CostA& operator=(const CostA& rhs) override;
    
    /// cost equality.
    /// @param rhs a cost to compare to.
    bool operator==(const CostA& rhs) const;
    
    /// a distance value, in percent of the bigger cost.
    /// used for approximate equality.
    double dist(const CostA& rhs) const;
    
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool operator<(const CostA& rhs) const;
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    virtual CostA& operator+=(const CostA& rhs);
    
    // null cost value.
    // CostA zero() const override { return CostA(); }
    
    /// create a new null cost value.
    virtual std::shared_ptr<Cost> shared_zero() const override;
    
    /// create a shared clone of this cost.
    virtual std::shared_ptr<Cost> shared_clone() const override;
    
    /// create a smart clone of this cost.
    virtual std::unique_ptr<Cost> unique_clone() const override;
    
    /// update this cost for doing a transition renaming one note (single
    /// or in chord) with the given parameters and in a given hypothetic global
    /// local tonalities.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// not known.
    void update(const enum NoteName& name,
                const enum Accid& accid,
                bool print,
                const Ton& gton, const Ton& lton = Ton()) override;
    
    // update this cost for doing a transition into the given config,
    // from its previous config, in a given hypothetic global tonality.
    // @see PSC.previous()
    // @param gton conjectured main (global) tonality (key signature).
    // void update(const PSC1& c, const PSEnum& e,
    //             const Ton& gton) override;
    
    // update this cost for doing a transition into the given config,
    // from its previous config,
    // in the given hypothetic global and local tonalities.
    // @see PSC.previous()
    // @param gton conjectured main (global) tonality (key signature).
    // @param lton conjectured local tonality.
    // void update(const PSC1& c, const PSEnum& e,
    //             const Ton& gton, const Ton& lton) override;
    
    // update this cost for doing a transition into the given chord config,
    // in a given hypothetic global tonality.
    // @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    // @param accid chosen alteration for the received pitch, in -2..2.
    // @param print whether the accidental must be printed in score.
    // @param nbocc nb of occurrence of the note (pitch class) in this chord.
    // @param gton conjectured main (global) tonality (key signature).
    // void update(const PSC2& c, const PSEnum& e,
    //             const enum NoteName& name, const enum Accid& accid,
    //             bool print, size_t nbocc,
    //             const Ton& gton) override;
    
    // update this cost for doing a transition renaming one note (single
    // or in chord) with the given parameters and in a given hypothetic global
    // local tonalities.
    // @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    // @param accid chosen alteration for the received pitch, in -2..2.
    // @param print whether the accidental must be printed in score.
    // @param gton conjectured main (global) tonality (key signature).
    // @param lton conjectured local tonality or undef tonlity if it is
    // not known.
    // void update(const PSC2& c, const PSEnum& e,
    //             const enum NoteName& name, const enum Accid& accid,
    //             bool print, size_t nbocc,
    //             const Ton& gton, const Ton& lton) override;
    
    /// @param o output stream where to print this cost.
    void print(std::ostream& o) const;
    
protected: // data
    
    /// cumulated number of printed accidentals.
    size_t _accid; // unsigned int
    
};

// CostA operator+(const CostA& c1, const CostA& c2);

std::ostream& operator<<(std::ostream& o, const CostA& c);


} // namespace pse

#endif /* CostA_hpp */

/// @}
