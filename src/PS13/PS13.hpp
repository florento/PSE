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
    PS13(bool dflag=true, size_t kpre = 33, size_t kpost = 23);
    
    /// destructor
    virtual ~PS13();
    
    /// compute the best pitch spelling for the input notes.
    /// @return whether computation was succesfull.
    bool spell();
    
private: // data
        
    /// Chromatic harmonic scale for each pitch class.
    std::vector<Scale> _scales;
    
    const size_t _Kpre;

    const size_t _Kpost;

    /// count the number of occurrences of the pitch-class c in [n-Kpre, n+Kpost]
    unsigned int count(int c, size_t n) const;
    
    void init_scales();
        
};


} // namespace pse

#endif /* PS13_hpp */

/// @}
