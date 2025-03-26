//
//  CostAT.hpp
//  pse
//
//  Created by Florent on 16/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef CostAT_hpp
#define CostAT_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "pstrace.hpp"
#include "CostA.hpp"
// #include "Costt.hpp"


namespace pse {


/// measure of cost extending CostA with 3 members for tie breaking.
class CostAT : public CostA
{
    
public: // construction
    
    /// null cost.
    /// @param tb_lex use lexicographic comparison for tie-breaking components.
    /// Otherwise make the sum of some tie-breaking components before
    /// comparison.
    CostAT(bool tb_lex=true);
    
    /// copy constructor.
    CostAT(const CostAT& rhs);
    
    /// destructor
    virtual ~CostAT();
    
    /// create a new null cost value.
    std::shared_ptr<Cost> shared_zero() const override
    { return Cost::shared_zero<CostAT>(this->_tblex); }
    
    /// create a shared clone of this cost.
    std::shared_ptr<Cost> shared_clone() const override
    { return Cost::shared_clone<CostAT>(); }
    
    /// create a smart clone of this cost.
    std::unique_ptr<Cost> unique_clone() const
    { return Cost::unique_clone<CostAT>(); }
    
public: // operators called in Cost

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const CostAT& rhs) const;
    
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const CostAT& rhs) const;
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    virtual CostAT& add(const CostAT& rhs);
    
    /// a distance value, in percent of the smaller cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const CostAT& rhs) const;
    
protected: // operators

    /// cost equality.
    /// @param rhs a cost to compare to.
    bool equal(const Cost& rhs) const override
    { return Cost::equal<CostAT>(rhs); }
        
    /// cost inequality.
    /// @param rhs a cost to compare to.
    bool smaller(const Cost& rhs) const override
    { return Cost::smaller<CostAT>(rhs); }
    
    /// cumulated sum operator. update this cost by adding rhs.
    /// @param rhs a cost to add.
    Cost& add(const Cost& rhs) override
    { return Cost::add<CostAT>(rhs); }
    
    /// a distance value, in percent of the smaller cost.
    /// used for approximate equality.
    /// @warning only used for selection of global (rowcost comparison).
    double pdist(const Cost& rhs) const override
    { return Cost::pdist<CostAT>(rhs); }
    
    /// equality for tie-breaking members.
    /// @param rhs a cost to compare to.
    bool tiebreak_equal(const CostAT& rhs) const;

    /// variant of equality for tie-breaking members
    /// with a member by member comparison.
    // @param rhs a cost to compare to.
    bool tiebreak_equal_lex(const CostAT& rhs) const;

    /// variant of equality for tie-breaking members
    /// with a sum of cflat and double.
    /// @param rhs a cost to compare to.
    bool tiebreak_equal_lex0(const CostAT& rhs) const;

    /// variant of equality for tie-breaking members with a sum of some members.
    /// - color + cflat + double
    /// - chromharm
    /// @param rhs a cost to compare to.
    bool tiebreak_equal_sum(const CostAT& rhs) const;

    /// inequality for tie-breaking members.
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members:
    /// - cflat + double
    /// - chromharm
    /// - color
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_lex00(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members:
    /// - cflat + double
    /// - color
    /// - chromharm
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_lex01(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members:
    /// - color
    /// - cflat + double
    /// - chromharm
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_lex02(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members:
    /// - chromharm
    /// - color
    /// - cflat + double
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_lex03(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members:
    /// - chromharm
    /// - cflat + double
    /// - color
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_lex04(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members
    /// with a lexicographic member by member comparison.
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_lex1(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members
    /// with a lexicographic member by member comparison.
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_lex2(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members
    /// with a sum of some members.
    /// - color + cflat + double
    /// - chromharm
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_sum(const CostAT& rhs) const;

    /// variant of inequality for tie-breaking members
    /// with a sum of some members.
    /// - chromharm
    /// - color + cflat + double
    /// @param rhs a cost to compare to.
    bool tiebreak_smaller_sum2(const CostAT& rhs) const;

    /// distance for tie-breaking members.
    /// @param rhs a cost to compare to.
    double tiebreak_pdist(const CostAT& rhs) const;

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

protected: // update members

    /// update the measure chromharm for this cost, with the given values.
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

    /// update the count of wrong color for this cost with the given values.
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

    /// update the count of Cb B# E# Fb for this cost, with the given values.
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

    /// update the count of double sharp and double flats for this cost,
    /// with the given values.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param gton conjectured main (global) tonality (key signature).
    /// ignored for CostA.
    /// @param lton conjectured local tonality or undef tonlity if it is
    /// unknown. ignored for CostA.
    /// @return wether an update was effectively performed.
    virtual bool updateDouble(const enum NoteName& name,
                              const enum Accid& accid,
                              bool print,
                              const Ton& gton, const Ton& lton = Ton());

private: // update convenience
    
    /// update the measure chromharm for this cost, with the given values.
    /// @param name chosen name for the received pitch.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param ton a ton for comparing accid to chromatic harmonic scale.
    /// must not be undef.
    /// @return wether an update was effectively performed.
    virtual bool updateChroma1(const enum NoteName& name,
                               const enum Accid& accid,
                               bool print,
                               const Ton& ton);
    
    /// update the count of wrong color for this cost with the given values.
    /// @param name chosen name for the received pitch.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param ton a ton for comparing color of accid. must not be undef.
    /// @return wether an update was effectively performed.
    virtual bool updateColor1(const enum NoteName& name,
                              const enum Accid& accid,
                              bool print,
                              const Ton& ton);

    /// update the count of Cb B# E# Fb for this cost, with the given values.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param ton a ton for checking presence of cflat in scale.
    /// must not be undef.
    /// @return wether an update was effectively performed.
    virtual bool updateCflat1(const enum NoteName& name,
                              const enum Accid& accid,
                              bool print,
                              const Ton& ton);

    /// update the count of double sharp and double flats for this cost,
    /// with the given values.
    /// @param accid chosen alteration for the received pitch.
    /// @param print whether the accidental must be printed in score.
    /// @param ton a ton for checking presence of double accid in scale.
    /// must not be undef.
    /// @return wether an update was effectively performed.
    virtual bool updateDouble1(const enum NoteName& name,
                               const enum Accid& accid,
                               bool print,
                               const Ton& ton);
    
public: // access and debug

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
    
    /// number of accidental not present in the chromatic harmonic scale.
    /// for tie-breaking.
    size_t _chromharm;
    
    /// number of non-lead accidentals
    /// with a color different from local ton or global ton (when local undef).
    /// for tie-breaking.
    size_t _color;

    /// number of non lead Cb B# E# Fb.
    /// for tie-breaking.
    size_t _cflat;

    /// number of non-lead double sharps or double flats.
    /// for tie-breaking.
    size_t _double;

    /// sum of the three last tie-breaking measures
    /// color, cflat and double
    size_t _tbsum;

    /// compare the tie-breaking components lexicographically,
    /// otherwise, make the sum of the three last tie-breaking components
    /// before comparison.
    bool _tblex;
    
protected: // debug

    /// print only the tie-breaking components
    /// @param o output stream where to print this cost.
    void printTB(std::ostream& o) const;
    
};

// CostA operator+(const CostA& c1, const CostA& c2);

std::ostream& operator<<(std::ostream& o, const CostAT& c);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostAT> : fmt::ostream_formatter {};

#endif /* CostAT_hpp */

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
