//
//  PSConfig.hpp
//  qparse
//
//  Created by Florent Jacquemard on 14/03/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSConfig_hpp
#define PSConfig_hpp


#include <iostream>
#include <assert.h>
#include <array>
#include <vector>

#include "trace.hpp"
#include "Accidental.hpp"
#include "PSConfig0.hpp"
#include "PSEnum.hpp"


namespace pse {


/// extension of PSConfig with annotations
/// for the note leading to this config from its predecessor:
/// - the note name
/// - a print flag
///
/// Configurations of this class are always non-initial in a best path solution
/// for pitch spelling.
class PSC : public PSC0
{
public:

    /// target PS config for a transition from given (previous) config,
    /// when reading a pitch.
    /// copy and update with given accident for given name and accidental,
    /// in given conjectured global tonality.
    /// only the cost (number of accidentals) is updated.
    /// @param c previous config (origin), to be updated with the received pitch.
    /// @param mp midi pitch to be named, used to update the config.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param ton conjectured main (global) tonality (key signature).
    PSC(const PSC0& c, unsigned int mp,
        const NoteName& name, const Accid& accid, const Ton& ton);

    /// alternative target PS config for a transition
    /// from a given (previous) PS config, when reading a pitch.
    /// copy and update with given accident for given name and accidental,
    /// in given conjectured global tonality and local tonality.
    /// the cost (number of accidentals) and distance (to local ton) are updated.
    /// @param c previous config, to be updated with the received pitch.
    // @param p pitch to be named, used to update the config.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param acc chosen alteration for the received pitch, in -2..2.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    PSC(const PSC0& c, unsigned int mp, const NoteName& name, const Accid& acc,
        const Ton& ton, const Ton& lton);

    /// copy constructor
    PSC(const PSC& c);
    
    virtual ~PSC();
    
    /// assignement operator
    PSC& operator=(const PSC& rhs);

    /// configs have  the same list of accidentals
    bool operator==(const PSC& rhs) const;
    
    /// configs have different list of accidentals
    bool operator!=(const PSC& rhs) const;
    
    /// always false: PSC are non-initial configurations in a best path.
    virtual bool initial() const;
    
    /// previous config in shortest path to this config,
    /// or null if there are none.
    /// every config has at most one predecessor.
    virtual const PSC0* previous() const;
    
    /// midi pitch of the note read for the transition
    /// from this config's predecessor.
    unsigned int midi() const;

    /// name of the note read for the transition from this config's predecessor.
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @see NoteName.hpp
    /// @warning not formatted for a Pitch object.
    NoteName name() const;
    
    /// accidental of the note read for the transition
    /// from this config's predecessor. in -2..2
    /// @see Accidental.hpp
    /// @warning not formatted for a Pitch object.
    Accid accidental() const;
    
    /// whether the accidental of the note (read for the transition from predecessor)
    /// must be printed or not.
    bool printed() const;
    
    // update the cumulated distance for this Config,
    // with the distance (in the array of fifths) to the given tonality.
    // @param ton a tonality.
    // @todo obsolete, TBR
    // void updateDist(const Ton& ton);

//    void succ(PSEnum& _enum, const Ton& ton, PSCQueue& q) const;
//
//    void succ(PSEnum& _enum, const Ton& ton, const Ton& lton,
//              PSCQueue& q) const;

    
private:
        
    /// previous config in shortest path to this config.
    const PSC0* _pred;
    
    /// MIDI pitch of the note read for the transition to this config.
    unsigned int _midi;

    /// a chosen pitch name, in 0..6 (0 is 'C', 6 is 'B'),
    /// for the note read for the transition to this config.
    NoteName _name;
    
    // a chosen number of accidents, in -2..2.
    // @todo TBR: it is _state[_name] by construction
    // int _accid;
    
    /// whether the accident must be printed
    /// for the note read for the transition to this config.
    bool _print;
    

private:

    // update the cumulated number of accidents for this Config,
    // with the number of printed alterations of this config.
    // void updateAccidents();

};


} // namespace pse

#endif /* PSConfig_hpp */

/// @}
