//
//  PSPath.hpp
//  squant2
//
//  Created by Florent Jacquemard on 16/02/2021.
//  Copyright © 2021 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSPath_hpp
#define PSPath_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <array>
#include <vector>
#include <stack>
#include <queue>   // std::priority_queue

#include "pstrace.hpp"
//#include "Pitch.hpp"
//#include "KeyFifth.hpp"
#include "PSState.hpp"
// #include "PSConfig.hpp"
//#include "NoteEnum.hpp"
//#include "PSNoteEnum.hpp"
#include "PSConfig0.hpp"
#include "PSConfig1.hpp"
#include "PSConfig1c.hpp"
//#include "PSConfig2.hpp"
#include "PSBag.hpp"


  
namespace pse {


/// Sequence of note names and accidentals computed by shortest path
/// pitch spelling algorithm from one sequence of notes.
/// The notes in the sequence are given by a part enumerator (see NoteEnum)
/// and the index (wrt the enumerator) of first and last note of sequence.
class PSP
{
public:
      
    // shortest path extracted, wrt for a given local tonality,
    // from a bag of best config computed
    // by the shortest path pitch spelling algorithm
    // from one sequence of notes read from a note enumerator,
    // with selection on cost only (nb of accidentals).
    // for every choice between configs of the bags,
    // the closest to the tonality is selected.

    /// shortest path extracted from a final configuration of a best path,
    /// computed with the given note enumerator.
    /// @param psc PS configuration.
    /// @param e an enumerator of notes for computing transitions between configs.
    /// the enumerator and the path targeting the configuration must
    /// have the same length.
    PSP(const PSC0& psc, PSEnum& e);

    ~PSP();


    size_t size() const;
    
    /// name for the pitch of the note of given index in the best path,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of note in enumerator, must be between first and last.
    const enum NoteName& name(size_t i) const;
    
    /// alteration for pitch of note of given index in the best path, in -2..2.
    /// @param i index of note in enumerator, must be between first and last.
    const enum Accid& alteration(size_t i) const;
    
    /// print flag for pitch of note of given index in the best path.
    /// @param i index of note in enumerator, must be between first and last.
    bool printed(size_t i) const;

    /// nb of accidents in best path from first to last note.
    /// For debug info.
    inline const Cost& cost() const; // { return _cost; }
    
    /// rename all notes read to build this PSP.
    void rename();
    
    void print(std::ostream& o) const;
       
private:
    
    // local tonality of the best path.
    //const Ton& _ton;
       
    // PSConfig* _final;

    // backup of visited nodes
    //std::vector<std::shared_ptr<const PSC0>> _visited;

    /// enumerator of notes for computing the best path.
    PSEnum& _enum;
    
    /// list of note names (in 0..6) on the best path from note n0 to note n1.
    std::vector<enum NoteName> _names;

    /// list of accidents (in -2..2) on the best path from note n0 to note n1.
    std::vector<enum Accid> _accids;

    /// list of flag print for the best path from note n0 to note n1.
    std::vector<bool> _prints;

    /// cumulated cost in the best path from n0 to n1.
    std::shared_ptr<Cost> _cost;
    
    /// best path was succesfull estimated
    bool _computed;
    
    /// compute the best path from the given bag of configs.
    /// @param ton an estimated global tonality (key signature).
    /// @param lton an estimated local tonality.
    /// @todo TBR obsolete
    bool init(const Ton& ton, const Ton& lton = Ton());

    // compute the bast path from the given bag of configs.
    // @param psb bag of best config, to compute one best path.
    // @todo TBR
    // void init(const PSB& psb);

    // @todo TBR
    // void init();

    // compute the shortest paths wrt nb of accidents and dist
    // @param queue2 contains the complete paths best wrt nb of accidents.
    // must not be empty.
    // @todo TBR
    // void init2(PSCQueue& queue2);
    
    void record_path(const PSC0& c);
    
    /// @param i index of note in enumerator, must be between first and last.
    void print(std::ostream& o, size_t i) const;
    
};

std::ostream& operator<<(std::ostream& o, const PSP& p);

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::PSP> : fmt::ostream_formatter {};

#endif /* PSPath_hpp */

/// @}
