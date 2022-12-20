//
//  PSConfig0.hpp
//  squant2
//
//  Created by Florent Jacquemard on 15/02/2021.
//  Copyright © 2021 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSConfig0_hpp
#define PSConfig0_hpp

#include <iostream>
#include <assert.h>
// #include <array>
#include <vector>
#include <stack>
#include <queue>   // std::priority_queue

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
//#include "Pitch.hpp"
//#include "KeyFifth.hpp"
#include "AccidentState.hpp"
#include "PSEnum.hpp"
#include "PSCost.hpp"


namespace pse {


class PSC0;

/// type of orderings for container of PS Configs
using PSCCompare = std::function<bool(std::shared_ptr<const PSC0>,
                                      std::shared_ptr<const PSC0>)>;

/// priority queue of PS Configs
typedef std::priority_queue<std::shared_ptr<const PSC0>,
                            std::vector<std::shared_ptr<const PSC0>>,
                            PSCCompare> PSCQueue;

/// Configuration for a pitch spelling algorithm of scope 1 bar.
/// Configurations of this class are always initial in a best path solution
/// for pitch spelling.
class PSC0
{
public:

    /// initial config for a given tonality.
    /// predecessor configuration will be null.
    /// @param ton a tonality, used to defined the accident state of this initial config.
    /// @param id index (in a note enumerator) of the note to read
    /// in order to reach the successor configs from this config.
    PSC0(const Ton& ton, size_t id);

    // initial config for a given key signature.
    // @param init index of last note read to reach this configuration.
    // PSC0(const KeyFifth& ks, size_t init=0);

    // initial config with a given accident state
    // @param init index of last note read to reach this configuration
    // PSC0(const AccidState& s, size_t init=0);

    /// copy constructor
    PSC0(const PSC0& s);

    virtual ~PSC0();
    
    /// assignement operator
    PSC0& operator=(const PSC0& rhs);

    /// configs have  the same list of accidentals
    bool operator==(const PSC0& rhs) const;
    
    /// configs have different list of accidentals
    bool operator!=(const PSC0& rhs) const;
    
    /// always true: PSC0 are initial configurations in a best path.
    virtual bool initial() const;
    
    /// previous config in shortest path to this config,
    /// or null if there are none.
    /// every config has at most one predecessor.
    virtual const PSC0* previous() const;

    /// accidental for the given note name recorded in this config.
    /// @param name note nate coded in 0..6 (0 is 'C', 6 is 'B').
    const Accid accidental(const NoteName& name) const;

    /// index (in an enumerator) of note read for the transition from
    /// this config to its successors.
    size_t id() const;

    /// cost of the minimal path to this config.
    inline const PSCost& cost() const { return _cost; }
    
    /// cumulated number of accidents in the minimal path to this config.
    inline int accidentals() const { return _cost.getAccid(); }
    
    /// cumulated distance to tonic in the minimal path to this config.
    /// @todo remove
    inline int dist() const { return _cost.getDist(); }

    /// cumulated number of non-conjoint moves in the minimal path to this config.
    /// @todo remove
    inline int disjoint() const { return _cost.getDisj(); }

    /// allocate every config reached by one transition from this config,
    /// when reading the given pitch, and push it to the given queue.
    // @param p pitch of note read to reach this configuration.
    /// @param e an enumerator of notes for computing transitions from
    /// this config.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param q queue receiving the target configs.
    void succ(PSEnum& e, const Ton& ton, PSCQueue& q) const;

    /// allocate every config reached by one transition from this config,
    /// when reading the given pitch, and push it to the given queue.
    /// @param e an enumerator of notes for computing transitions from
    /// this config.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    /// @param q queue receiving the target configs.
    void succ(PSEnum& e, const Ton& ton, const Ton& lton, PSCQueue& q) const;

    // stack the path of PS configs
    // from initial config (top or stack) to this PS config,
    // following PS0::previous().
    // std::stack<const PSC0*> stack() const;
    
    // allocate and return the list of target configs reached by transition
    // from this config, when receiving the given pitch.
    // std::vector<std::shared_ptr<PSC0>>
    //                            successors(const Pitch& p, int ton) const;

    // origin of the best path this configuration belongs to.
    // virtual const PSC0* origin() const;
        
protected:

    /// description of accidents for each note name.
    AccidState _state;

    // description of discounted accident for each note name.
    // will not be updated.
    // AccidState _jokers;
    
    /// index (in an enumerator) of note read for the transition from
    /// this config to its successors.
    /// @todo mv to PSC
    size_t _id;
    
    /// cumulated cost in the minimal path to this config.
    PSCost _cost;
    
    // cumulated number of accidents in the minimal path to this config.
    // unsigned int _accidents;
    
    // cumulated distance to a conjectured local tonality
    // in the minimal path to this config.
    // unsigned int _dist;

    // cumulated number of non-conjoint moves
    // in the minimal path to this config.
    // unsigned int _disj;

};






} // namespace pse

#endif /* PSConfig0_hpp */

/// @} 
