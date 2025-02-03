//
//  Weber.hpp
//  pse
//
//  Created by Florent Jacquemard on 17/12/2022.
//
/// @addtogroup pse
/// @{

#ifndef Weber_hpp
#define Weber_hpp


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
#include "Weber_static.hpp"


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
class Weber
{
public:

    /// main constructor
    Weber();

    /// destructor
    ~Weber();

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
  
    /// internal alias
    static const size_t WS_NBTONS = 30; // Weber_static::NB_TONS;
    
    /// internal alias
    static const int WS_UNDEFDIST; //  = Weber_static::UNDEF_DIST;
        
    // array of tonalities considered.
    // const std::array<const Ton, NB_TONS> TON;

    /// matrix of distances between tonalities.
    /// triangular.
    std::array<std::array<int, WS_NBTONS>, WS_NBTONS> DIST;
    
private:

    /// compute the matrix of distances.
    void init();
    
    // size_t first: index of ton j
    // int second: length smallest path so far from i to j (can be UNDEF_DIST)
    // typedef std::pair<size_t, int> pairi;
    
    /// extract from heap the index with min value in dist and return it.
    /// @param heap bitvector of index present.
    /// @param dist vector of distance values (positive or UNDEF_DIST = +infinity).
    /// @return index with min value in dist and present in heap or
    /// NB_TONS if there are none.
    static size_t extractMin(std::array<bool, WS_NBTONS>& heap,
                       const std::array<int,  WS_NBTONS>& dist);

    static bool isEmpty(const std::array<bool, WS_NBTONS>& heap);

    /// jth neighbour of the tonality of index i.
    /// @param i index of tonality. must be smaller than NB_TONS.
    /// @param j neighbour number.
    ///        - 0 for up
    ///        - 1 for left
    ///        - 2 for right
    ///        - 3 for down
    /// @return the index of the neighbour tonality of NB_TONS if there is none.
    static size_t neighbour_tonal(size_t i, size_t j);

};


} // end namespace pse
    
#endif /* Weber_hpp */

/// @} // end group pse
