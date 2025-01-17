//
//  Weber_static.hpp
//  pse
//
//  Created by Florent Jacquemard on 04/03/2024.
//
/// @addtogroup pse
/// @{


#ifndef Weber_static_hpp
#define Weber_static_hpp


#include <iostream>
#include <assert.h>
#include <array>
#include <vector>
#include <queue>

#include "pstrace.hpp"
#include "NoteName.hpp"
//#include "Accidental.hpp"
//#include "Fifthspse::Weber
//#include "KeyFifth.hpp"
#include "Ton.hpp"


namespace pse {


class Weber_static
{
public:

    /// undefined distance value.
    static const int UNDEF_DIST; // = -1;

    /// number of tonalities considered.
    static const size_t NB_TONS;
    
    /// Weber distance between two tonalities,
    /// extracted from the static distance matrix WEBER_DIST
    /// precomputed with the functions of this class.
    /// @param ton1 tonality. must be major or harmonic minor,
    ///        key signature in -7..7.
    /// @param ton2 tonality. must be major or harmonic minor,
    ///        key signature in -7..7.
    /// @return positive int or UNDEF_DIST.
    /// @see WEBER_DIST
    static int dist(const Ton& ton1, const Ton& ton2);
    
    /// index of the given tonality in the array of tonalities considered,
    /// or NB_TONS if it does not belong to the array.
    static size_t index(const Ton& ton);
       
    /// `a < b` where `a` and `b` are positive or `UNDEF_DIST` (+infinity).
    static bool inf(int a, int b);
    
private:
        
    /// precomputed matrix of Weber distance between tonalities.
    /// obtained by the functions of the class
    /// @see init()
    /// @see dump()
    static const std::vector<std::vector<int>> WEBER_DIST;
    // static const std::array<std::array<int, NB_TONS>, NB_TONS> WEBER_DIST;
    
};

} // end namespace pse
    
#endif /* Weber_static_hpp */

/// @} // end group pse
