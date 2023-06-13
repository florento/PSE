//
//  PS13.hpp
//  pse
//
//  Created by Florent Jacquemard on 29/03/2023.
//
/// @addtogroup pitch
/// @{

#ifndef PS13_hpp
#define PS13_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <array>
#include <vector>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Mode.hpp"
#include "ModeFactory.hpp"
#include "Scale.hpp"
#include "PSRawEnum.hpp"
#include "Speller.hpp"

namespace pse {

/// wrapper on top of PSRawEnum implementing D. Meredith PS13 algorithm.
/// interface to pybind.
class PS13 : public Speller
{
public:

    
    /// main constructor. initially empty list of notes to spell.
    /// @param dflag debug mode.
    /// @param kpre parameter of PS13
    /// @param kpost parameter of PS13
    /// @see http://www.titanmusic.com/papers/public/ps-ircam.pdf section 10
    PS13(size_t kpre = 33, size_t kpost = 23, bool dflag=true);

    // constructor with default values kpre=33, kpost = 23
    // PS13(bool dflag);

    // default constructor,
    // default values kpre=33, kpost = 23, debug = false
    // PS13();

    
    /// destructor
    virtual ~PS13();
    
    // name of algorithm implemented
    // @return Algo::PS13 for this class.
    // virtual Algo algo() const override;
    
    /// set the Kpre parameter value.
    void setKpre(size_t kpre) { _Kpre = kpre; }
    
    /// set the Kpost parameter value.
    void setKpost(size_t kpost) { _Kpost = kpost; }

    /// compute the best pitch spelling for the input notes.
    /// @return whether computation was succesfull.
    bool spell() override;
    
    /// estimated global tonality.
    /// @warning it is always an undef Ton for PS13.
    const Ton& global() const override; //  { return Ton(); }

    /// index of estimated global tonality.
    /// @warning it is always an undef Ton for PS13.
    size_t iglobal() const override;

    // estimated local tonality at note of given index.
    // @param i index of note in the list of input notes.
    // @warning spell() must have been called.
    // virtual const Ton& local(size_t i) const override;

private: // data
        
    /// Chromatic harmonic scale for each pitch class.
    std::vector<Scale> _scales;
    
    /// Kpre parameter of PS13 (tonal window prefix).
    size_t _Kpre;

    /// Kpost parameter of PS13 (tonal window postfix).
    size_t _Kpost;
    
    /// estimated global tonality.
    /// @warning undef for PS13.
    const Ton _global;
    
    // one estimated local tonality for each note.
    // std::vector<Ton> _locals;

    /// count the number of occurrences of the pitch-class c in [n-Kpre, n+Kpost)
    size_t count(int c, size_t n) const;
    
    void init_scales();
        
};


} // namespace pse

#endif /* PS13_hpp */

/// @}
