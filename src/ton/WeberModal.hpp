//
//  WeberModal.hpp
//  pse
//
//  Created by Florent Jacquemard on 17/12/2022.
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

#include "trace.hpp"
#include "NoteName.hpp"
//#include "Accidental.hpp"
//#include "Fifthspse::Weber
//#include "KeyFifth.hpp"
#include "Ton.hpp"


namespace pse {

/// computation of a table of distance between tonalities, following
/// Jacob Gottfried Weber
/// Versuch eine geordneten Theorie des Tonsetzkunst
/// (1817-1821)
///
/// the table of relationships between keys:
///
///  Eb  c  C  a  A  f# F# d# D# b#
///
///  Ab  f  F  d  D  b  B  g# G# e#
///
///  Db  bb Bb g  G  e  E  c# C# a#
///
///  Gb  eb Eb c  C  a  A  f# F# d#
///
///  Cb  ab Ab f  F  d  D  b  B  g#
///
///  Fb  db Db bb Bb g  G  e  E  c#
///
///  Bbb gb Gb eb Eb c  C  a  A  f#
class WeberModal
{
public:

    /// undefined distance value.
    static const int UNDEF_DIST;

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
    
    
    /// Weber distance between two tonalities,
    /// extracted from the static distance matrix WEBER_DIST
    /// precomputed with the functions of this class.
    /// @param ton1 tonality. must be major or harmonic minor,
    ///        key signature in -7..7.
    /// @param ton2 tonality. must be major or harmonic minor,
    ///        key signature in -7..7.
    /// @return positive int or UNDEF_DIST.
    /// @see WEBER_DIST
    static int static_dist(const Ton& ton1, const Ton& ton2);
    
private: // data

    /// number of tonalities considered.
    static const size_t NB_TONS = 30;

    // array of tonalities considered.
    // const std::array<const Ton, NB_TONS> TON;

    /// matrix of distances between tonalities.
    /// triangular.
    std::array<std::array<int, NB_TONS>, NB_TONS> DIST;
    
    /// precomputed matrix of Weber distance between tonalities.
    /// obtained by the functions of the class
    /// @see init()
    /// @see dump()
    static const std::array<std::array<int, NB_TONS>, NB_TONS> WEBER_DIST;
    
private:
    
    /// compute the matrix of distances.
    void init();
    
    /// size_t first: index of ton j
    /// int second: length smallest path so far from i to j (can be UNDEF_DIST)
    typedef std::pair<size_t, int> pairi;

    /// `a < b` where `a` and `b` are positive or `UNDEF_DIST` (+infinity).
    static bool inf(int a, int b);
    
    /// extract from heap the index with min value in dist and return it.
    /// @param heap bitvector of index present.
    /// @param dist vector of distance values (positive or UNDEF_DIST = +infinity).
    /// @return index with min value in dist and present in heap or
    /// NB_TONS if there are none.
    static size_t extractMin(std::array<bool, NB_TONS>& heap,
                      const std::array<int, NB_TONS>& dist);

    static bool isEmpty(const std::array<bool, NB_TONS>& heap);

    /// index of the given tonality in the array of tonalities considered,
    /// or NB_TONS if it does not belong to the array.
    static size_t index(const Ton& ton);
    
    /// jth neighbour of the tonality of index i.
    /// @param i index of tonality. must be smaller than NB_TONS.
    /// @param j neighbour number.
    ///        - 0 for up
    ///        - 1 for left
    ///        - 2 for right
    ///        - 3 for down
    /// @return the index of the neighbour tonality of NB_TONS if there is none.
    static size_t neighbour_tonal(size_t i, size_t j);
    static size_t neighbour(size_t i, size_t j);

};


} // end namespace pse
    
#endif /* WeberModal_hpp */

/// @} // end group pse


