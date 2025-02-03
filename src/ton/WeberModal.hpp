//
//  WeberModal.hpp
//  pse
//
//  Created by Florent Jacquemard on 27/02/2024.
//
/// @addtogroup pse
/// @{

#ifndef WeberModal_hpp
#define WeberModal_hpp

#include <iostream>
#include <assert.h>
#include <array>
#include <vector>
#include <queue>

#include "pstrace.hpp"
#include "NoteName.hpp"
//#include "Accid.hpp"
//#include "Fifthspse::Weber
//#include "KeyFifth.hpp"
#include "Ton.hpp"
#include "WeberModal_static.hpp"
#include "WeberBluesModal_static.hpp"

namespace pse {

class WeberModal
{
public:

    // undefined distance value.
    // static const int UNDEF_DIST;

    /// main constructor
    WeberModal();

    /// destructor
    ~WeberModal();

    /// Weber distance between two tonalities.
    /// @param ton1 tonality. must be major or harmonic minor,
    ///        key signature in -7..7.
    /// @param ton2 tonality. must be major or harmonic minor,
    ///        key signature in -7..7.
    /// @return positive int or UNDEF_DIST.
    int dist(const Ton& ton1, const Ton& ton2);

    /// print the matrix of distances.
    void dump() const;
    
private: // data

    // number of tonalities considered.
    // static const size_t NB_TONS = 165;
    
    /// internal alias
    static const size_t WMS_NBTONS = 135; // WeberModal_static::NB_TONS;
    
    /// internal alias
    static const int WS_UNDEFDIST; //  = Weber_static::UNDEF_DIST;

    // array of tonalities considered.
    // const std::array<const Ton, WMS_NBTONS> TON;

    /// matrix of distances between tonalities.
    /// triangular.
    std::array<std::array<int, WMS_NBTONS>, WMS_NBTONS> DIST;
    
private:
    
    /// compute the matrix of distances.
    void init();
    
    /// `a < b` where `a` and `b` are positive or `UNDEF_DIST` (+infinity).
    static bool inf(int a, int b);
    
    /// extract from heap the index with min value in dist and return it.
    /// @param heap bitvector of index present.
    /// @param dist vector of distance values (positive or UNDEF_DIST = +infinity).
    /// @return index with min value in dist and present in heap or
    /// NB_TONS if there are none.
    static size_t extractMin(std::array<bool, WMS_NBTONS>& heap,
                       const std::array<int,  WMS_NBTONS>& dist);

    static bool isEmpty(const std::array<bool, WMS_NBTONS>& heap);

    /// index of the given tonality in the array of tonalities considered,
    /// or NB_TONS if it does not belong to the array.
    static size_t index_modal(const Ton& ton);
    
    /// jth neighbour of the tonality of index i.
    /// @param i index of tonality. must be smaller than NB_TONS.
    /// @param j neighbour number.
    ///        - 0 for up
    ///        - 1 for left
    ///        - 2 for right
    ///        - 3 for down
    ///        - 4 for back
    ///        - 5 for front
    /// @return the index of the neighbour tonality of NB_TONS if there is none.
    static size_t neighbour_modal(size_t i, size_t j);

};

} // end namespace pse

#endif /* WeberModal_hpp */

/// @} // end group pse
