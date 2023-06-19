//
//  PSTransit.hpp
//  pse
//
//  Created by Florent on 11/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef Transition_hpp
#define Transition_hpp

#include <iostream>
#include <assert.h>
#include <vector>

#include "trace.hpp"
#include "AlgoName.hpp"
#include "PSEnum.hpp"
#include "PSCost.hpp"
#include "PSOrder.hpp"
#include "PSConfig0.hpp" // PSC0, PSCQueue
#include "PSConfig1.hpp"
#include "PSConfig1c.hpp"
#include "PSConfig2.hpp"


namespace pse {


/// computation of configurations (PSC0) reached from other configurations
/// by transition, when when reading one pitch or several simultaneous pitchs,
/// according to the different pitch spelling algorithms implemented.
class Transition
{
public:
    
    /// @param a name of algorithm to consider for computing the transitions.
    /// @param e an enumerator of notes for computing transitions between configs.
    Transition(const Algo& a, PSEnum& e);

    /// allocate every config reached by one transition from the given config,
    /// when reading one pitch or several simultaneous pitchs,
    /// and push it to the given queue.
    /// @param c source configuration.
    /// this config.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality. ignored if algo is not PSE1.
    /// @param q queue receiving the target configs.
    void succ(std::shared_ptr<const PSC0> c,
              const Ton& ton, const Ton& lton, PSCQueue& q) const;
    
    /// allocate every config reached by one transition from the given config,
    /// when reading one pitch and push it to the given queue.
    /// @param c source configuration.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality. ignored if algo is not PSE1.
    /// @param q queue receiving the target configs.
    void succ1(std::shared_ptr<const PSC0> c,
               const Ton& ton, const Ton& lton, PSCQueue& q) const;

    // allocate every config reached by one transition from the given config,
    // when reading several simultaneous pitchs (chord), and push it to
    // the given queue.
    // @param c source configuration.
    // this config.
    // @param ton conjectured main (global) tonality (key signature).
    // @param lton conjectured local tonality. ignored if algo is not PSE1.
    // @param q queue receiving the target configs.
    // @todo former version, to be replaced
    // void succ2_TBR(std::shared_ptr<const PSC0> c,
    //           const Ton& ton, const Ton& lton, PSCQueue& q) const;

    /// allocate every config reached by one transition from the given config,
    /// when reading several simultaneous pitchs (chord), and push it to
    /// the given queue.
    /// @param c source configuration.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality. ignored if algo is not PSE1.
    /// @param q queue receiving the target configs.
    /// @todo replacement of succ2_TBR
    void succ2(std::shared_ptr<const PSC0> c,
               const Ton& ton, const Ton& lton, PSCQueue& q) const;
    
private:

    /// name  of algorithm to consider for computing the transitions.
    Algo _algo;

    /// enumerator of notes for computing transitions between configs.
    PSEnum& _enum;
    
};



} // namespace pse

#endif /* Transition_hpp */

/// @}

