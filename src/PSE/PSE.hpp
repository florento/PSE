//
//  PSE.hpp
//  pse
//
//  Created by Florent Jacquemard on 29/03/2023.
//
/// @addtogroup pitch
/// @{

#ifndef PSE_hpp
#define PSE_hpp

#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"
#include "TonIndex.hpp"
#include "PSRawEnum.hpp"
#include "Speller.hpp"
#include "PSTable.hpp"


namespace pse {

/// wrapper on top of PSRawEnum.
/// interface to pybind.
class PSE : public Speller
{
public:
    
    /// main constructor. initially empty list of notes to spell.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see PSTable
    PSE(size_t nbTons=0, bool dflag=true);

    /// destructor
    virtual ~PSE();

    /// number of tonalities considered for pitch spelling.
    size_t nbTons() const;

    /// tonality (for pitch spelling) of given index.
    /// @param i an index in array of tonalities. must be smaller than nbtons().
    /// @see nbTons()
    const Ton& ton(size_t i) const;
    
    /// empty the array of tonalities considered for pitch-spelling vectors.
    /// @see addTon
    void resetTons();
    
    /// add one tonality to the array of tonalities considered for
    /// pitch-spelling vectors.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of the tonality added.
    /// @see Ton
    void addTon(int ks, ModeName mode = ModeName::Major);

    /// add a tonality for pitch spelling.
    void addTon(const Ton& ton);
        
    /// compute the best pitch spelling for the input notes.
    /// @return whether computation was succesfull.
    bool spell();
    
    /// force global tonality. it wont be estimated.
    /// @param i index of tonality set as global.
    void setGlobal(size_t i);

    /// number of candidates (ties) for the estimatation of the global tonality.
    size_t globalCands() const;
    
    /// candidate global tonality for this table.
    /// @param i candidate number, must be in 0..globalCands().
    const Ton& globalCand(size_t i) const;

    /// index of a candidate global tonality for this table, in 0..index.size().
    size_t iglobalCand(size_t i) const;
 
    /// estimated local tonality for one candidate global tonality and one bar.
    const Ton& local(size_t i, size_t j) const; // { return _table.local(i, j); }
    
    /// estimated global tonality
    /// @warning respell() must have been called.
    const Ton& global() const;
    
    /// index of the estimated global tonality.
    size_t iglobal() const;
    
    /// @return distance in the array of fifths between
    /// from estimated global tonality and
    /// a signature with no accidentals.
    inline int fifths() const { return global().fifths(); }

private: // data
        
    /// Pitch Spelling table
    PST _table;
    
    // evaluated global tonality
    // Ton _global;
    
private:
        
    // default array of tonalities considered for Pitch Spelling.
    // @todo mv to TonIndex
    // static const std::vector<const Ton> TONS;

    // smaller table of all tonalities considered for PS.
    // @todo mv to TonIndex
    // static const std::vector<const Ton> TONS26;

    // table of state vectors associated to tonalities in TON.
    // static std::array<const PSState, NBTONS> ASTATES;

    // table of joker state vectors associated to tonalities in TON.
    // static std::array<const PSState, NBTONS> AJOKER;

};


} // namespace pse

#endif /* PSE_hpp */

/// @}
