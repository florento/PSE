//
//  TonIndex.hpp
//  pse
//
//  Created by Florent Jacquemard on 24/02/2023.
//
/// @addtogroup pitch
/// @{


#ifndef TonIndex_hpp
#define TonIndex_hpp

#include <iostream>
#include <assert.h>
#include <array>
#include <vector>
#include <memory>

#include "trace.hpp"
//#include "Pitch.hpp"
#include "KeyFifth.hpp"
#include "Ton.hpp"

namespace pse {

/// index (list of row headers) for PSTable and PSVector (column in table).
/// The array of tonalities for PS is encapsulated,
/// and every tonality is indexed in 0..TonIndex::size()
/// and can be accessed with TonIndex::ton(i).
class TonIndex
{
public:
    
    /// maximal number of tonalities considered for Pitch Spelling.
    static const size_t MAXTONS = 900;
    
    /// a tonality that was ont estimated yet.
    static const size_t UNDEF = MAXTONS+1;

    /// a tonality whose estimation failed.
    static const size_t FAILED = MAXTONS+2;
    
    /// main constructor
    /// @param nb default list of tonalities.
    /// currently supported
    /// - 0  : empty list
    /// - 26 : major and harmonic minor, KS between -6 and 6.
    /// - 30 : major and harmonic minor, KS between -7 and 7.
    /// - 25 : tonalities of Bach's Wohltemperierte Clavier
    ///        major      KS -4 to 7 : C, C#, D, Eb, E, F, F#, G, Ab, A, Bb, B
    ///        minor harm KS -6 to 6 : C, C#, D, Eb, D#, E, F, F#, G, G#, A, Bb, B
    TonIndex(size_t nb=0);

    /// destructor
    virtual ~TonIndex();

    
    /// number of tons in this index.
    size_t size() const;

    /// no tons in this index.
    bool empty() const;

    /// Tonality corresponding to the given row index.
    /// @param i an index in array of tonalities. must be smaller than NBTONS.
    const Ton& ton(size_t i) const;
    
    /// empty this index of tonalities.
    /// @see addTon
    void reset();

    ///add a tonality to this index of row headers.
    void add(const Ton& ton);
    
    ///add a tonality to this index of row headers.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of this tonality.
    /// @see Ton
    void add(int ks, const Ton::Mode& mode = Ton::Mode::Maj);
    
private: // data
    
    /// vector of tonalities
    std::vector<const Ton> _tons;

    /// default array of 13 tonalities, ks between -6 and 6.
    /// @param mode must not be Ton::Mode::Undef.
    void init13(const Ton::Mode& mode);

    /// default array of 15 tonalities, ks between -7 and 7.
    /// @param mode must not be Ton::Mode::Undef.
    void init15(const Ton::Mode& mode);
    
    /// array of tonalities of Bach's Wohltemperierte Clavier.
    /// major          KS -4 to 7 : C, C#, D, Eb, E, F, F#, G, Ab, A, Bb, B
    /// minor harmonic KS -6 to 6 : C, C#, D, Eb, D#, E, F, F#, G, G#, A, Bb, B
    void init25();


    // default array of tonalities considered for Pitch Spelling.
    // static const std::vector<const Ton> TONS30;
    
    // smaller table of all tonalities considered for PS.
    // static const std::vector<const Ton> TONS26;
    
    /// @todo TONS25
    
};


} // namespace pse

#endif /* TonIndex_hpp */

/// @}
