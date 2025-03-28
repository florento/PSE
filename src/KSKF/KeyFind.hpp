//
//  KeyFind.hpp
//  qparse
//
//  Created by Florent Jacquemard on 06/12/2020.
//  Copyright © 2020 Florent Jacquemard. All rights reserved.
//
// Krumhansl-Schmuckler key-finding algorithm
// (with Pearson's correlation coefficient)
// @see http://rnhart.net/articles/key-finding/
// @see http://extra.humdrum.org/man/keycor/#description
//
/// @addtogroup pitch
/// @{

#ifndef KeyFind_hpp
#define KeyFind_hpp

#include <stdio.h>
#include <assert.h>
#include <math.h>       // sqrt 
#include <vector>
#include <array>

#include "pstrace.hpp"
#include "MTU.hpp"
#include "Pitch.hpp"
#include "Part.hpp"
//#include "AEVisitor.hpp"
//#include "NoteEnum.hpp"


namespace scoremodel {


/// Initial configuration for a pitch spelling algorithm of rescope 1 bar
class KeyFind
{
public:

    /// @param p a part in a score model. cannot be changed.
    /// @todo replace by const Part& when there is a const_iterator
    KeyFind(Part& p);

    /// copy constructor
    KeyFind(const KeyFind& kf);
    
    ~KeyFind();

    /// key signature found.
    /// the number of fifths distance from a signature with no accidentals.
    int signature() const;

    /// whether the key signature found is major.
    /// @return true for major key, false for minor key.
    bool major();
    
private:
    
    /// part concerned with key finding
    /// @todo replace by const Part& when there is a const_iterator
    Part& _part;
    
    /// total duration for every note class in the current part, in MTU.
    std::array<mtu_t, 12>  _durr;

    /// total duration for every note class in the current part, in float.
    std::array<double, 12>  _dury;

    /// mean of the table _dury
    double _meany;
    
    /// the number of fifths distance
    /// from a signature with no accidentals.
    int _sig;

    /// true for major key, false for minor key.
    bool _major;
    
    static int SIG_UNDEF;
    
    /// Krumhansl-Schmuckler table of profiles
    /// for every note class (starting at C = 0), for major keys,
    /// according to http://rnhart.net/articles/key-finding/
    static const std::array<double, 12> KS_MAJOR_PROFILE;
    
    static const double KS_MAJOR_MEAN;

    /// Krumhansl-Schmuckler table of profiles
    /// for every note class (starting at A), for minor keys,
    /// according to http://rnhart.net/articles/key-finding/
    static const std::array<double, 12> KS_MINOR_PROFILE;

    static const double KS_MINOR_MEAN;

    /// compute the table of durations
    void init_dur();

    /// correlation coefficient for a given key, major or minor.
    /// @param key must be between -7 7
    /// @param major true for major, false for minor key
    double correlation_coefficient(int key, bool major);

    /// compute the key signature with KS algorithm.
    void init_sig();

};


} // namespace scoremodel

#endif /* KeyFind_hpp */

/// @}

