//
//  PSVector.hpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSVector_hpp
#define PSVector_hpp

#include <iostream>
#include<sstream>
#include <assert.h>
#include <array>
#include <vector>
#include <memory>

#include "trace.hpp"
//#include "MTU.hpp"
//#include "Pitch.hpp"
//#include "Part.hpp"
//#include "AEVisitor.hpp"
//#include "NoteEnum.hpp"
#include "KeyFifth.hpp"
#include "Ton.hpp"
#include "PSEnum.hpp"
#include "PSBag.hpp"
#include "PSPath.hpp"


namespace pse {


/// vector with one PSP (shortest path of of PSC)
/// for each tonality considered for Pitch Spelling.
/// The list of tonalities for PS is encapsulated,
/// @see PSV::NBTONS
/// @see PSV::TONS
/// The notes in the sequence are given by a part enumerator
/// and the index (wrt the enumerator) of first and last note of sequence.
/// @see NoteEnum
class PSV
{

public:

    /// number of tonalities considered for Pitch Spelling.
    /// It is the static length of every PS Vector.
    /// The list of tonalities for PS is encapsulated,
    /// every tonality is indexed in 0..NBTONS
    /// and can be accessed with PSV::ton(i).
    static const size_t NBTONS = 30;
    
    /// main constructor.
    /// @param tons vector of tonalities. dimension of this vector.
    /// @param e an enumerator of notes for transitions of configs.
    /// @warning the enumerator cannot be changed once the object created.
    PSV(const std::vector<const Ton>& tons,
        const PSEnum& e);
    
    /// main constructor.
    /// @param tons vector of tonalities. dimension of this vector.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param i0 index of the first note to read in enumerator.
    /// @param i1 index of the note after the last note to read in enumerator.
    /// must be superior of equal to i0.
    /// the sequence is empty iff if i0 == i1.
    PSV(const std::vector<const Ton>& tons,
        const PSEnum& e, size_t i0, size_t i1);
    
    /// main constructor.
    /// @param tons vector of tonalities. dimension of this vector.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param i0 index of the first note to read in enumerator.
    /// The enumeration starts at i0 and stops
    /// when there are no more notes to read in e.
    PSV(const std::vector<const Ton>& tons,
        const PSEnum& e, size_t i0);
    
    ~PSV();
  
    /// Dimension of this vector.
    inline size_t nbtons() const;

    /// Tonality corresponding to the given index.
    /// @param i an index in array of tonalities. must be smaller than NBTONS.
    const Ton& ton(size_t i) const;

    /// bag of target configs for best paths for the ton of given index,
    /// according to the cost of path (nb of accidentals)
    /// @param i index in array of tonalities. must be smaller than NBTONS.
    const PSB& best0(size_t i) const;

    /// bag of target configs for best paths for the ton of given index,
    /// according to
    /// - the cost of path (nb of accidentals), and
    /// - the distance to local tonality.
    /// @param i index in array of tonalities. must be smaller than NBTONS.
    const PSB& best1(size_t i);
    
    inline size_t first() const { return psenum().first(); }
    inline size_t stop()  const { return psenum().stop(); }

    /// index of the estimated local tonality for this vector, in 0..NBTONS-1.
    /// @return the estimated local tonality for this vector,
    /// or NBTONS in case of error.
    /// @warning estimLocal() must have been called successfully.
    size_t local() const;
    
    /// estimate the local tonality from PS bags in this vector.
    /// @param iton index of the local tonality of the previous PSV
    /// or global tonality for the initial PSV.
    /// @return whether estimation of the local tonality successed.
    bool estimateLocal(size_t iton);
    
    /// rename all notes read to build this PS vector.
    /// local tonality is estimated if this was not done before.
    /// @param i index in array of tonalities. must be smaller than NBTONS.
    /// @return whether renaming succeeded for this measure.
    bool rename(size_t i);
    
private:
    
    /// vector of tonalities = dimension of this vector
    const std::vector<const Ton>& _tons;

    /// enumerator of notes transmitted to embedded PSB's.
    const std::unique_ptr<PSEnum> _enum;

    /// one bag of best paths (target configs) per ton,
    /// paths are ordered according to their cost (nb of accidentals).
    std::array<std::unique_ptr<const PSB>, NBTONS> _psb0;

    /// one bag of best paths (target configs) per ton,
    /// paths are ordered according to
    /// - their cost (nb of accidentals),
    /// - the distance to local tonality.
    std::array<std::unique_ptr<const PSB>, NBTONS> _psb1;

    /// index of local tonality in TONS
    size_t _local;
    
    /// the local tonality has been estimated
    bool _estimated;
    
    /// debug counter: nb of tie break fails.
    unsigned int _tiebfail;
    
    
private:

    /// access private PS enumerator
    PSEnum& psenum() const;

    /// fill the vector psb0 with PS Bags constructed with the notes enumerated.
    void init();
    
    /// distance 1 for tie break in estimation of local tonality
    unsigned int bestDist(size_t prev, const PSC0& psc);

};


} // namespace pse

#endif /* PSVector_hpp */

/// @}
