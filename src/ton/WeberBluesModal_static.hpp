//
//  WeberBluesModal_static.hpp
//  pse
//
//  Created by Augustin Bouquillard on 21/07/2024.
//
/// @addtogroup pse
/// @{

#ifndef WeberBluesModal_static_hpp
#define WeberBluesModal_static_hpp

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


class WeberBluesModal_static
{
public:

    /// number of tonalities considered.
    static const size_t NB_TONS = 165;
    
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
    
    /// ton of given index in the array of tonalities considered,
    /// or undefined tonality Ton() if the index is out of range.
    /// inverse of fonction index,
    /// @param i index of a ton in the array of tonalities considered.
    /// must be smaller than NB_TONS.
    static const Ton ton(size_t i);
    
    /// mode of the ton of given index in the array of tonalities considered,
    /// or ModeName::Undef if the index is out of range.
    /// @param i index of a ton in the array of tonalities considered.
    /// must be smaller than NB_TONS.
    static ModeName mode(size_t i);
    
    /// distance, in the array of fifths, between the key signature of the ton
    /// of given index, in the array of tonalities considered,
    /// and a signature with no accidentals.
    /// @param i index of a ton in the array of tonalities considered.
    /// must be smaller than NB_TONS.
    /// @return fifth distance or KeyFifth::UNDEF if the index is out of range.
    static int fifths(size_t i);

private:
        
    /// precomputed matrix of Weber distance between tonalities.
    /// obtained by the functions of the class
    /// @see init()
    /// @see dump()
    //static const std::array<std::array<int, NB_TONS>, NB_TONS> WEBER_DIST;
    static const std::vector<std::vector<int>> WEBER_DIST;

};

} // end namespace pse
    
#endif /* WeberBluesModal_static_hpp */

/// @} // end group pse
