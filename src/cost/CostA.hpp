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
// #include "Costt.hpp"


namespace pse {

class PSC1;

class PSC2;


/// measure of cost defined by the cumulated number of printed accidentals.
class CostA : public Cost // public PolymorphicCost<CostA>
{
    
public: // construction
    
    /// null cost.
    // @param discount apply or not a discount (during update) for accidentals
    // in the assumed  scale (lead degrees).
    CostA(); // bool discount=false
    
    /// copy constructor.
    CostA(const CostA& rhs);
    
    /// distructor
    virtual ~CostA();
    
    // assignement operator.
    // @param rhs a cost to copy.
    // CostA& operator=(const CostA& rhs) override;

    /// create a new null cost value.
    std::shared_ptr<Cost> shared_zero() const override;
    
    /// create a shared clone of this cost.
    std::shared_ptr<Cost> shared_clone() const override;
    
    /// create a smart clone of this cost.
    std::unique_ptr<Cost> unique_clone() const;
    
protected: // operators

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const CostA& rhs) const;

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const Cost& rhs) const override;
        
    // equality for tie-breaking members.
    // @param rhs a cost to compare to.
    bool tiebreak_equal(const CostA& rhs) const;

    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const Cost& rhs) const override;
    
    // inequality for tie-breaking members.
    // @param rhs a cost to compare to.
    bool tiebreak_smaller(const CostA& rhs) const;

    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    virtual CostA& add(const CostA& rhs);

    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    virtual Cost& add(const Cost& rhs) override;

    /// a distance value, in percent of the smaller cost.
    /// used for approximate equality.
    double pdist(const Cost& rhs) const override;
    
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
    /// @return wether an update was effectively performed.
    bool update(const enum NoteName& name,
                const enum Accid& accid,
                bool print,
                const Ton& gton, const Ton& lton = Ton()) override;

protected: // update member

    /// update the member accid of this cost with the given values.
    /// @param name chosen name for the received pitch.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// ignored for CostA.
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// unknown. ignored for CostA.
    virtual bool updateAccid(const enum NoteName& name,
                             const enum Accid& accid,
                             bool print,
                             const Ton& gton, const Ton& lton = Ton());
    
    /// update the member chromharm of this cost with the given values.
    /// @param name chosen name for the received pitch.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// ignored for CostA.
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// unknown. ignored for CostA.
    /// @return wether an update was effectively performed.
    virtual bool updateChroma(const enum NoteName& name,
                              const enum Accid& accid,
                              bool print,
                              const Ton& gton, const Ton& lton = Ton());

    /// update the member color of this cost with the given values.
    /// @param name chosen name for the received pitch.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// ignored for CostA.
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// unknown. ignored for CostA.
    /// @return wether an update was effectively performed.
    virtual bool updateColor(const enum NoteName& name,
                             const enum Accid& accid,
                             bool print,
                             const Ton& gton, const Ton& lton = Ton());

    /// update the member cflat for the received pitch.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// ignored for CostA.
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// unknown. ignored for CostA.
    /// @return wether an update was effectively performed.
    virtual bool updateCflat(const enum NoteName& name,
                             const enum Accid& accid,
                             bool print,
                             const Ton& gton, const Ton& lton = Ton());
        
public: // access and debug

    /// accessor for debug.
    inline size_t get_accid() const { return _accid; }

    inline size_t get_chromharm() const { return _chromharm; }

    /// accessor for debug.
    inline size_t get_color() const { return _color; }

    /// accessor for debug.
    inline size_t get_cflat() const { return _cflat; }
    
    /// Cost type of this const value.
    virtual CostType type() const override;
    
    /// @param o output stream where to print this cost.
    void print(std::ostream& o) const override;
    
protected: // data
    
    /// cumulated number of printed accidentals.
    size_t _accid; // unsigned int
    
    /// is the accid present in the chromatic harmonic scale?
    /// for tie-breaking.
    size_t _chromharm;
    
    /// cumulated number of accidentals with color different from global ton.
    /// for tie-breaking.
    size_t _color;

    /// cumulated number of printed and non lead Cb B# E# Fb.
    /// for tie-breaking.
    size_t _cflat;
    
};

// CostA operator+(const CostA& c1, const CostA& c2);

std::ostream& operator<<(std::ostream& o, const CostA& c);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostA> : fmt::ostream_formatter {};

#endif /* CostA_hpp */

/// @}

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
