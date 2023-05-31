//
//  PSBag.hpp
//  qparse
//
//  Created by Florent Jacquemard on 10/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSBag_hpp
#define PSBag_hpp

#include <iostream>
#include <assert.h>
#include <vector>

#include "trace.hpp"
//#include "MTU.hpp"
#include "AlgoName.hpp"
//#include "Part.hpp"
//#include "AEVisitor.hpp"
//#include "Pitch.hpp"
#include "PSEnum.hpp"
#include "Cost.hpp"
#include "PSConfig0.hpp"
#include "PSConfig1.hpp"
#include "PSConfig2.hpp"


namespace pse {


// PSCCompare defined in PSConfig0.hpp
// PSCQueue defined in PSConfig0.hpp

/// set of PS Configs
typedef std::vector<std::shared_ptr<const PSC0>> PSCHeap;

/// bag of target PS Configs for best paths for a tonality and a sequence
/// of notes.
/// - all the configs in the bag have the same source
///   (initial config for the tonality).
/// - all the configs in the bag have the same number of accidentals (best nb).
class PSB
{
public:
    
    // bag of best target configs configs for a conjectured global tonality
    // (key sig) and a measure of notes.
    // @param ton conjectured global tonality (key sig),
    // used to define the initial config.
    // @param e an enumerator of notes for computing transitions between configs.
    // @todo TBR
    // PSB(const Ton& ton, PSEnum& e, const Algo& a);
    
    /// bag of best target configs configs for a conjectured global tonality
    /// (key sig), a conjectured local tonality (tie break) and a measure of notes.
    /// @param a name of pitch-spelling algorithm implemented.
    /// @param seed cost value of specialized type used to create a null cost
    /// of the same type.
    /// @param e an enumerator of notes for computing transitions between configs.
    /// @param ton conjectured global tonality (key sig),
    /// used to define the initial config.
    /// @param lton conjectured local tonality, to compute the cumulated
    /// distance value used for tie break.
    PSB(const Algo& a, const Cost& seed,
        PSEnum& e, const Ton& ton, const Ton& lton);
    
    ~PSB();
    
    /// whether this bag is empty.
    bool empty() const;
    
    /// number of PS Configs i this bag
    size_t size() const;
    
    /// cost of the best path in this bag.
    const Cost& cost() const;
    
    /// access one PS config in this bag.
    /// @warning this bag must not be empty.
    const PSC0& top() const;
    
    // remove the top PS config of this bag.
    // @warning this bag must not be empty.
    // void pop();
    
    /// iterator to the pointing to the first element in this bag.
    PSCHeap::const_iterator cbegin() const;
    
    /// iterator to the pointing to the past-the-end element in this bag.
    PSCHeap::const_iterator cend() const;
    
    /// rename all notes in input used to build this bag,
    /// according to the best path in the bag.
    /// @return whether renaming succeeded for this bag.
    bool rename();
    
private: // data
    
    // tonality of the best path, determines the source config of the best paths.
    // const Ton& _ton;

    /// enumerator of notes for computing transitions between configs.
    PSEnum& _enum;
    
    /// bag of final configs of best paths.
    PSCHeap _bests;

    // bag of non-final configs involved in best paths.
    // for rollback of best path.
    // useless: they are stored in _prev
    // std::vector<std::shared_ptr<const PSC0>> _inbests;

    /// cost of the best config in the bag.
    std::shared_ptr<Cost> _cost;

    // backup of visited non-terminal nodes (pointed as previous).
    // std::vector<std::shared_ptr<const PSC0>> _visited;
    // @todo TBR

private:

    /// compute and add the shortest paths in this bag.
    /// two computation algorithms possible (selected with a flag):
    /// - shortest path determined only by the nb of accidents.
    /// - shortest path determined by the nb of accidents and distance
    ///   to a conjectured local tonality (tie break).
    /// @param ton conjectured global tonality (key sig),
    /// used to define the initial config.
    /// @param lton conjectured local tonality, to compute the cumulated
    /// dist value used for tie break.
    /// @param a name of pitch-spelling algorithm implemented.
    // @param fsucc flag, whether the successor is computed with ton only
    // or ton and lton.
    void init(const Ton& ton, const Ton& lton,
              const Algo& a, const Cost& seed); // bool fsucc);

    // @param q priority queue for the computation of the best path.
    
    /// access one PS config in this bag.
    /// @param i index of an element of this bag.
    /// must be strictly smaller than PSB.size().
    /// @warning this bag must not be empty.
    const PSC0& at(size_t i) const;
    
    // TBR unused
    // void addBest(std::shared_ptr<const PSC0>& c);
    
};


} // namespace pse

#endif /* PSBag_hpp */

/// @}
