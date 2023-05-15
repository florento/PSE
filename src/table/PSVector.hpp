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
#include <sstream>
#include <assert.h>
#include <array>
#include <vector>
#include <memory>

#include "trace.hpp"
//#include "MTU.hpp"
#include "AlgoName.hpp"
//#include "Pitch.hpp"
//#include "Part.hpp"
//#include "AEVisitor.hpp"
//#include "NoteEnum.hpp"
#include "KeyFifth.hpp"
#include "Ton.hpp"
#include "TonIndex.hpp"
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

    /// @todo param step number
    /// main constructor.
    /// @param a name of pitch-spelling algorithm implemented.
    /// @param index array of tonalities. dimension of this vector.
    /// @param e an enumerator of notes for transitions of configs.
    /// @warning the enumerator cannot be changed once the object created.
    PSV(const Algo& a, const TonIndex& index, const PSEnum& e);
    
    /// @todo param step number
    /// main constructor.
    /// @param a name of pitch-spelling algorithm implemented.
    /// @param index array of tonalities. dimension of this vector.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param i0 index of the first note to read in enumerator.
    /// @param i1 index of the note after the last note to read in enumerator.
    /// must be superior of equal to i0.
    /// the sequence is empty iff if i0 == i1.
    PSV(const Algo& a, const TonIndex& index,
        const PSEnum& e, size_t i0, size_t i1);
    
    /// @todo param step number
    /// main constructor.
    /// @param a name of pitch-spelling algorithm implemented.
    /// @param index array of tonalities. dimension of this vector.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param i0 index of the first note to read in enumerator.
    /// The enumeration starts at i0 and stops
    /// when there are no more notes to read in e.
    PSV(const Algo& a, const TonIndex& index, const PSEnum& e, size_t i0);
    
    ~PSV();
  
    /// first id of enumerator
    inline size_t first() const { return psenum().first(); }

    /// one after last id of enumerator
    inline size_t stop()  const { return psenum().stop(); }

    /// bag of target configs for best paths for the ton of given index.
    /// @param step number of step, 0 or 1.
    /// @param i index in array of tonalities. must be smaller than index.size().
    const PSB& best(size_t step, size_t i);

    /// estimate a local tonality, for a given assumed global tonality,
    /// from a given previous vector (vector for previous measure).
    /// @param prev previous vector. Its local tonalities must have been evaluated.
    /// @return whether estimation of the local tonalities successed.
    bool estimateLocal(size_t ig, const PSV& prev);

    /// estimate a local tonality, for a given assumed global tonality,
    /// and a given previous local tonality (for previous measure).
    /// @param ig index in the TonIndex of an assumed global tonality.
    /// @param iprev index of the previous local tonality for ig.
    /// @return whether estimation of the local tonality successed.
    bool estimateLocal(size_t ig, size_t iprev);

    /// estimate a local tonality, for a given assumed global tonality,
    /// without prior (when this vector is the first measure).
    /// @return whether estimation of the local tonalities successed.
    bool estimateLocal(size_t ig);
       
    /// index (in the TonIndex) of the estimated local tonality,
    /// for the current bar,
    /// assuming a global tonality (also represented by an index).
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @return the estimated local tonality for the global tonality i:
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    /// @warning estimLocal() must have been called successfully.
    size_t local(size_t i) const;
    
    /// rename all notes read to build this PS vector.
    /// local tonality is estimated if this was not done before.
    /// @param i index in array of tonalities. must be smaller than index.size().
    /// @return whether renaming succeeded for this measure.
    bool rename(size_t i);
    
private: // data
    
    /// name  of the pitch spelling algorithm implemented.
    const Algo& _algo;
    
    /// header of rows: vector of tonalities.
    /// copy of the index of the embedding table.
    const TonIndex& _index;
        
    // vector of tonalities = dimension of this vector
    // const std::vector<const Ton>& _tons;
    // @todo replace by TonIndex ref

    /// enumerator of notes transmitted to embedded PSB's.
    const std::unique_ptr<PSEnum> _enum;

    /// vector of bags of best paths (target configs), one bag for each ton.
    /// best paths computed with partial ordering.
    std::vector<std::shared_ptr<const PSB>> _psb_partial;

    /// vector of bags of best paths (target configs), one bag for each ton.
    /// best paths computed with total ordering.
    std::vector<std::shared_ptr<const PSB>> _psb_total;

    /// index (in the TonIndex) of the estimated local tonality, for each
    /// global tonality (also represented by an index).
    /// _local[i] is the estimated best local tonality for the current bar,
    /// assuminng that i is the index of the globabl tonality.
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    std::vector<size_t> _local;
    
    /// debug counter: nb of tie break fails for estimation of local ton.
    size_t _tiebfail;
    
    // the local tonality has been estimated
    // @todo rm or estimated()
    // bool _estimated;

private:

    /// access private PS enumerator
    PSEnum& psenum() const;

    /// fill the vector psb0 with PS Bags constructed with the notes enumerated.
    void init();
    
    /// distance 1 for tie break in estimation of local tonality
    unsigned int bestDist(size_t prev, const PSC0& psc);
    
    // Dimension of this vector.
    // inline size_t nbtons() const;

    /// Tonality corresponding to the given index.
    /// @param i an index in array of tonalities. must be smaller than NBTONS.
    const Ton& ton(size_t i) const;

    /// return value for best step 1 in case of error.
    const PSB& best1ERROR(size_t i) const;
    
};


} // namespace pse

#endif /* PSVector_hpp */

/// @}
