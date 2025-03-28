//
//  CostAD.hpp
//  pse
//
//  Created by Florent Jacquemard on 13/06/2023.
//
/// @addtogroup pitch
/// @{

#ifndef CostAD_hpp
#define CostAD_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "pstrace.hpp"
#include "CostAT.hpp"
// #include "Costt.hpp"


namespace pse {


/// abstract cost class with the following cumulated measures:
/// - number of printed accidentals.
/// - distance to a conjectured local tonality.
/// - number of accidentals not present in the chromaticharmonic scale.
/// - number of accidentals with color different from the assumed  global tonality.
/// - number of printed and non lead Cb B# E# Fb.
/// This class is used for the definitions of:
/// - class CostADplus : public PolymorphicComparable<CostAD, CostADplus>
/// - class CostADlex : public PolymorphicComparable<CostAD, CostADlex>
/// @todo inherit from CostA ?
class CostAD : public CostAT 
{
    
public: // construction
    
    /// null cost.
    /// @param tb_lex use lexicographic comparison for tie-breaking components.
    /// Otherwise make the sum of some tie-breaking components before
    /// comparison.
    CostAD(bool tb_lex=true);

    /// copy constructor.
    CostAD(const CostAD& rhs);
    
    /// distructor
    virtual ~CostAD();
    
public: // operators called in Cost
    
    /// componentwise cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const CostAD& rhs) const;
    
    /// update this cost by adding rhs componentwise.
    /// @param rhs a cost to add.
    Cost& add(const CostAD& rhs);
    
    // a distance value, in percent of the bigger cost.
    // used for approximate equality.
    // @todo RM already defined in Cost
    // virtual double pdist(const CostAD& rhs) const = 0;
       
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
    bool update(const enum NoteName& name,
                const enum Accid& accid,
                bool print,
                const Ton& gton, const Ton& lton = Ton(),
                const enum NoteName& prev_name = NoteName::Undef) override;
    
    /// version of the TENOR article
    virtual void update_tonale(const enum NoteName& name,
                               const enum Accid& accid,
                               bool print,
                               const Ton& gton, const Ton& lton = Ton());

    /// backup previous version (bug)
    virtual void update99(const enum NoteName& name,
                          const enum Accid& accid,
                          bool print,
                          const Ton& gton, const Ton& lton = Ton());

protected: // update
    
    /// update the member color of this cost with the given values.
    /// @param name chosen name for the received pitch.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// ignored for CostA.
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// unknown. ignored for CostA.
    /// @return wether an update was effectively performed.
    virtual bool updateDist(const enum NoteName& name,
                            const enum Accid& accid,
                            bool print,
                            const Ton& gton, const Ton& lton = Ton());
    
    
public: // access, debug

    /// accessor for debug.
    inline size_t get_dist() const { return _dist; }
    
    /// @param o output stream where to print this cost.
    void print(std::ostream& o) const override;
    
protected: // data
    
    // cumulated number of printed accidentals.
    // size_t _accid; // unsigned int
    
    /// cumulated distance to a conjectured local tonality.
    size_t _dist;
    
};

// CostAD operator+(const CostAD& c1, const CostAD& c2);

std::ostream& operator<<(std::ostream& o, const CostAD& c);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostAD> : fmt::ostream_formatter {};

#endif /* CostAD_hpp */


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

// update this cost for doing a transition into the given chord config,
// in given hypothetic global and local tonalities.
// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
// @param accid chosen alteration for the received pitch, in -2..2.
// @param print whether the accidental must be printed in score.
// @param nbocc nb of occurrence of the note (pitch class) in this chord.
// @param gton conjectured main (global) tonality (key signature).
// @param lton conjectured local tonality.
// void update(const PSC2& c, const PSEnum& e,
//             const enum NoteName& name, const enum Accid& accid,
//             bool print, size_t nbocc,
//             const Ton& gton, const Ton& lton) override;
