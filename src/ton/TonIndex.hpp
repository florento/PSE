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

#include "pstrace.hpp"
#include "utils.hpp"
//#include "Pitch.hpp"
#include "KeyFifth.hpp"
#include "ModeName.hpp"
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
    static const size_t MAXTONS;
    
    /// a tonality that is not present in this array of tonalities.
    static const size_t UNDEF; // = MAXTONS+1;

    /// a tonality whose estimation failed.
    static const size_t FAILED; // = MAXTONS+2;
    
    /// main constructor.
    /// @param nb default list of tonalities.
    /// currently supported:
    /// - 0  : empty list. tons can be added with add()
    /// - 25 : tonalities of Bach's Wohltemperierte Clavier
    ///   major      KS -4 to 7 : C, C#, D, Eb, E, F, F#, G, Ab, A, Bb, B
    ///   minor harm KS -6 to 6 : C, C#, D, Eb, D#, E, F, F#, G, G#, A, Bb, B
    /// - 26 : major and harmonic minor, KS between -6 and 6.
    /// - 30 : major and harmonic minor, KS between -7 and 7.
    /// - 104: major, minor, greek modes, KS in -6..6
    /// - 135: major, minor, greek modes, KS in -7..7
    /// - 165: modal: major, minor, greek modes, blues maojr and minor, KS in -7..7
    /// @warning the empty array of tonalities (case nb=0) is not closed
    /// (close() must be called aterwards). All the others are closed.
    TonIndex(size_t nb=0);
    
    /// destructor.
    virtual ~TonIndex();
    
    /// number of tons in this array of tonalities.
    size_t size() const;

    /// there are no tons in this array of tonalities.
    bool empty() const;

    /// Tonality at the given index in this array of tonalities.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    const Ton& ton(size_t i) const;

    /// Tonal or modal representative Ton in the equivalence class of the Ton
    /// af the given index in this array of tonalities.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    /// @param tonal mode: tonal or modal, for the construction of initial state
    /// (from the ton at i).
    /// @return the first ton in this tonIntdex equivalent to i, wrt
    const Ton& representative(size_t i, bool tonal) const;

    /// Index of the tonal or modal representative Ton in the equivalence class
    /// of the Ton af the given index in this array of tonalities.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    /// @param tonal mode: tonal or modal, for the construction of initial state
    /// (from the ton at i).
    size_t irepresentative(size_t i, bool tonal) const;

    /// Whether the tonality at the given index can be considered as global.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    bool global(size_t i) const;

    /// find the index of a ton in current array of tonalities.
    /// @param ton a given tonality.
    /// @return the index of ton or UNDEF if not found.
    size_t find(const Ton& ton) const;
        
    /// Enharmonic tonality corresponding to the given index.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    /// @return the following index:
    /// - i if the tonality i in this array has no enharmonics,
    /// - j != i if the tonality j in this array is an enharmonic of i.
    /// - UNDEF if if the tonality i in this array has an enharmonic that is not
    ///   present in this array.
    /// - FAIL in case of error.
    size_t enharmonic(size_t i) const;

    /// empty this index of tonalities, and rebuild it with
    /// the given default number of tonalities
    /// The array is unclosed.
    /// @param n number of default list of tonalities.
    /// @see constructor TonIndex
    /// @see addTon
    /// @warning with n=0, close() must be called aterwards.
    void reset(size_t n=0);

    ///add a tonality to this array of tonalities.
    /// @param ton the tonality to add to this index.
    /// @param global whether ton can be considered as a global tonality.
    /// @warning this array of tonalities must not be closed.
    void add(const Ton& ton, bool global=true);
    
    /// add a tonality to this array of tonalities.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of the tonality to add.
    /// @param global whether the tonality can be considered as global.
    /// @see Ton
    void add(int ks, const ModeName& mode = ModeName::Major, bool global=true);

    /// switch to tonal mode for the conmputation of Weber distance.
    /// @warning this array must not be closed.
    void setTonal();

    /// switch to modal mode for the computation of Weber distance.
    /// @warning this array must not be closed.
    void setModal();
    
    /// close this array of tonalities and finish initlialization.
    /// No ton can be added after closure.
    void close();

    /// this array of tonalities is closed (no ton can be added).
    bool closed() const;

    /// distance between the two tons of given indices,
    /// in the table of Weber, or WeberModal, or WeberBluesModal,
    /// avvording to the content of this array of tonalities.
    int distWeber(size_t i, size_t j) const;
    
    /// ranks of second given ton wrt Weber distance to first given ton.
    /// @param i index of ton in this array of tonalities.
    /// @param j index of ton in this array of tonalities.
    /// @return the rank of j in the vector of Weber distances to ton i of
    /// all the tons of this array of tonalities.
    /// @warning this array of tonalities must be closed.
    /// @todo remplacer par dist
    size_t rankWeber(size_t i, size_t j) const;
    
private: // data
    
    /// vector of tonalities.
    /// every tonality is associated a flag saying whether
    /// it can be considered as global.
    std::vector<std::pair<const Ton, bool>> _tons;

    /// vector of index of representative of each ton,
    /// in the class of tonal equivalent tons.
    std::vector<size_t> _repr_tonal;

    /// vector of index of representative of each ton,
    /// in the class of modal equivalent tons.
    std::vector<size_t> _repr_modal;

    /// ranks of ton wrt Weber distance:
    /// _rankWeber[i, j] is the rank of ton i in this index, wrt to
    /// to the distance to ton j in this index.
    /// tabulation for speedup
    std::vector<std::vector<size_t>> _rankWeber;
    
    /// this ton index has been closed:
    bool _closed;

    /// true if we use the tonal (original) Weber distance,
    /// false if we use the modal Weber distance.
    /// @todo TBR not used, replaced by _WeberModal and _WeberBlues
    bool _WeberTonal;

    /// this array of tonalities contains at least one ton with greek mode.
    bool _WeberModal;

    /// this array of tonalities contains at least one ton with a blues mode.
    bool _WeberBluesModal;
      
private:
    
    /// find the index of a ton defined by given key signature and mode
    /// in the current array of tonalities.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode a mode name.
    /// @return the index of ton or UNDEF if not found.
    size_t find(int ks, const ModeName& mode) const;

    /// @param n number of default list of tonalities.
    /// @see constructor TonIndex
    void init(size_t n);
    
    /// default array of 13 tonalities, ks between -6 and 6.
    /// @param mode must not be ModeName::Undef.
    /// @param global whether ton can be considered as a global tonality.
    void init13(const ModeName& mode, bool global);

    /// default array of 15 tonalities, ks between -7 and 7.
    /// @param mode must not be ModeName::Undef.
    /// @param global whether ton can be considered as a global tonality.
    void init15(const ModeName& mode, bool global);
    
    /// array of tonalities of Bach's Wohltemperierte Clavier.
    /// major          KS -4 to 7 : C, C#, D, Eb, E, F, F#, G, Ab, A, Bb, B
    /// minor harmonic KS -6 to 6 : C, C#, D, Eb, D#, E, F, F#, G, G#, A, Bb, B
    void init25();
    void initmodal();
    
    /// initialize the table _rankWeber of rank wrt Weber distance.
    void initRankWeber();
    
    /// a ton in the given mode can be considered global.
    /// for automatically constructed arrays.
    static bool global(const ModeName& mode);
    
    // static bool pcompare(const std::pair <size_t, unsigned int>& a,
    //                      const std::pair <size_t, unsigned int>& b);
    
    // static std::vector<size_t>
    // getRanks(std::vector<std::pair <size_t, unsigned int>>& v);
    
    // static std::vector<size_t> getRanks(const std::vector<unsigned int>& v);
    
    // default array of tonalities considered for Pitch Spelling.
    // static const std::vector<const Ton> TONS30;
    
    // smaller table of all tonalities considered for PS.
    // static const std::vector<const Ton> TONS26;
        
};

} // namespace pse

#endif /* TonIndex_hpp */

/// @}
