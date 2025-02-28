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

#include "pstrace.hpp"
#include "Cost.hpp"


namespace pse {


/// concrete measure of cost defined by the number of printed accidentals.
class CostA : public Cost
{
    
public: // construction
    
    /// null cost.
    CostA();
    
    /// copy constructor.
    CostA(const CostA& rhs);
    
    /// destructor
    virtual ~CostA();

    /// create a new null cost value.
    std::shared_ptr<Cost> shared_zero() const override
    { return Cost::shared_zero<CostA>(); }
    
    /// create a shared clone of this cost.
    std::shared_ptr<Cost> shared_clone() const override
    { return Cost::shared_clone<CostA>(); }

    /// create a smart clone of this cost.
    std::unique_ptr<Cost> unique_clone() const
    { return Cost::unique_clone<CostA>(); }

public: // operators called in Cost

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const CostA& rhs) const;

    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const CostA& rhs) const;

    /// update this cost by adding rhs.
    /// @param rhs a cost to add.
    CostA& add(const CostA& rhs);

    /// a distance value, in percent of the smaller cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const CostA& rhs) const;

protected: // operators

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const Cost& rhs) const override
    { return Cost::equal<CostA>(rhs); }

    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const Cost& rhs) const override
    { return Cost::smaller<CostA>(rhs); }

    /// update this cost by adding rhs.
    /// @param rhs a cost to add.
    Cost& add(const Cost& rhs) override
    { return Cost::add<CostA>(rhs); }

    /// a distance value, in percent of the smaller cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const Cost& rhs) const override
    { return Cost::pdist<CostA>(rhs); }

    // equality for tie-breaking members.
    // @param rhs a cost to compare to.
    bool tiebreak_equal(const CostA& rhs) const;

    // inequality for tie-breaking members.
    // @param rhs a cost to compare to.
    bool tiebreak_smaller(const CostA& rhs) const;
    
    // distance for tie-breaking members.
    // @param rhs a cost to compare to.
    double tiebreak_pdist(const CostA& rhs) const;
    
public: // update

    /// update this cost for doing a transition renaming one note (single
    /// or in chord) with the given parameters and in a given hypothetic global
    /// local tonalities.
    /// @param name chosen name for the received pitch,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// ignored for CostA.
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// unknown. ignored for CostA.
    /// @param prev_name previous name associated to has been associated to
    /// the received pitch (before processing it). Notename::Undef if the pitch
    /// was never assiated a name in the configuration's state.
    /// @return wether an update was effectively performed.
    bool update(const enum NoteName& name,
                const enum Accid& accid,
                bool print,
                const Ton& gton, const Ton& lton = Ton(),
                const enum NoteName& prev_name = NoteName::Undef) override;

protected: // update member

    /// update the number accids of this cost with the given values.
    /// @param name chosen name for the received pitch.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// ignored for CostA.
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// unknown. ignored for CostA.
    /// @return wether an update was effectively performed.
    virtual bool updateAccid(const enum NoteName& name,
                             const enum Accid& accid,
                             bool print,
                             const Ton& gton, const Ton& lton = Ton());
   
    /// update the number of naming inconsistencies for this cost
    /// with the given values.
    /// @param name chosen name for the received pitch.
    /// @param prev_name name associated to the received pitch prior to
    /// reception.
    /// @return wether an update was effectively performed.
    bool updateInconsistency(const enum NoteName& prev_name,
                             const enum NoteName& name);

public: // access and debug

    /// accessor for debug.
    inline size_t get_accid() const { return _accid; }

    /// Cost type of this const value.
    virtual CostType type() const override;
    
    /// @param o output stream where to print this cost.
    void print(std::ostream& o) const override;
    
protected: // data
    
    /// cumulated number of printed accidentals.
    size_t _accid; // unsigned int
    
    /// number of inconsist association of name to pitch class
    /// durring processing of current bar.
    size_t _inconsist;    
};

// CostA operator+(const CostA& c1, const CostA& c2);

std::ostream& operator<<(std::ostream& o, const CostA& c);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostA> : fmt::ostream_formatter {};

#endif /* CostA_hpp */

/// @}

// class PSC1;
// class PSC2;

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
