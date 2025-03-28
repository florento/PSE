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

class PST;

/// array of tonalities used for spelling
/// and every tonality is indexed in 0..TonIndex::size()
/// and can be accessed with TonIndex::ton(i).
/// some tonalities are possible candidates global
/// @see TonIndex::isGlobal(i)
///
/// **Life cycle of Ton Index**
/// - after creation, the ton index is open.
/// - tons can be added with add() and a global flag.
/// - at closure with TonIndex::close(),
///   the tons in the array are reordered.
/// - after closure, no ton can be added.
/// - global flags can be changed with TonIndex::selectGlobals()
///   and TonIndex::selectGlobal().
/// - TonIndex::resetGlobals() will restore global flags
///   to their state at closure.
/// - TonIndex::reset() clear the whole array of tonalities.
///
/// @see PSTable and PSVector: used as an index (list of row headers)
/// for these classes (column in table).
class TonIndex
{
    
public: // constants
    
    /// maximal number of tonalities considered for Pitch Spelling.
    static const size_t MAXTONS;
    
    /// a tonality that is not present in this array of tonalities.
    static const size_t UNDEF; // = MAXTONS+1;

    /// a tonality whose estimation failed.
    static const size_t FAILED; // = MAXTONS+2;

public: // construction

    /// main constructor.
    /// @param nb default list of tonalities.
    /// currently supported:
    /// - 0  : empty list. tons can be added with add()
    /// - 24 : major and harmonic minor, KS between -5 and 6.
    /// - 25 : tonalities of Bach's Wohltemperierte Clavier
    ///   major      KS -4 to 7 : C, C#, D, Eb, E, F, F#, G, Ab, A, Bb, B
    ///   minor harm KS -6 to 6 : C, C#, D, Eb, D#, E, F, F#, G, G#, A, Bb, B
    /// - 26 : major and harmonic minor, KS between -6 and 6.
    /// - 30 : major and harmonic minor, KS between -7 and 7.
    /// - 104: major, minor, greek modes, KS in -6..6
    /// - 135: major, minor, greek modes, KS in -7..7
    /// - 165: modal: major, minor, church modes, blues major and minor, KS in -7..7
    /// @warning the empty array of tonalities (case nb=0) is not closed
    /// (close() must be called aterwards). All the others are closed.
    TonIndex(size_t nb=0);

    /// a ton index cannot be copied.
    TonIndex(const TonIndex& rhs) = delete;
    
    /// destructor.
    virtual ~TonIndex();
    
    /// a ton index cannot be copied.
    TonIndex& operator=(const TonIndex& rhs) = delete;

public: // incremental construction
    
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
    
    /// close this array of tonalities and finish initlialization.
    /// No ton can be added after closure.
    void close();

    /// this array of tonalities is closed (no ton can be added).
    bool closed() const;

public: // access

    /// number of tons in this array of tonalities.
    size_t size() const;

    /// there are no tons in this array of tonalities.
    bool empty() const;

    /// Tonality at the given index in this array of tonalities.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    /// @see inverse of TonIndex::find.
    const Ton& ton(size_t i) const;

    /// reference to an undef ton.
    const Ton& undef() const;
   
    /// find the index of a ton in current array of tonalities.
    /// @param ton a given tonality.
    /// @return the index of ton or UNDEF if not found.
    /// @see inverse of TonIndex::ton.
    size_t find(const Ton& ton) const;
        
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

public: // sub array of tonalities that can be global.
    
    /// Whether the tonality at the given index can be considered as global.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    bool isGlobal(size_t i) const;
    
    /// number of tons in this array of tonalities that can be
    /// candidates estimated global.
    size_t globals() const;

    /// index of the n-best candidate global tonality.
    /// @param n number of candidate global tonality,
    /// must be in 0..globals().
    /// @return the index of the n-best global tonality
    /// in this index of tons, in 0..index.size(),
    /// or TonIndex::UNDEF in case of error.
    size_t getGlobal(size_t n = 0) const;

    /// Estimate and store the global tonality candidates
    /// according to the costs in the given table.
    /// @param tab for the extraction of global tonality.
    /// The index of tonalities stored refer to the ton index of this table.
    /// @param d tolerance distance (in percent) for considering two
    /// cost approximatively equal in the global ton search.
    /// @param refine whether we consider only current global tons or all tons
    /// of this ton index in the selection of new globals.
    /// in the first case, the new set of globals is a subset of the current
    /// one, in the second case, it may be orthogonal.
    /// @return the number of tonalities selected by this function.
    // @warning call eGlobals_eq and eGlobals_less variants to operator==
    // and operator< on cost.
    size_t selectGlobals(const PST& tab, double d=0, bool refine=true);
    
    /// select in the global tons the unique ton defined by bestGlobal()
    /// and unselect the others.
    /// @return whether the operation was succesful.
    /// @see bestGlobal()
    bool selectGlobal();

    /// select in the global tons a unique index on ton,
    /// according to the ordering defined in compare().
    /// @return the selected best global tonality in tie or
    /// TonIndex::UNDEF if tie break failed.
    size_t bestGlobal() const;
    size_t bestGlobal_old() const;

    /// restore the initial list of global tons (before selection of globals).
    void resetGlobals();
    
    /// set the given ton as the only possible global ton (unselect all others).
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode a mode name.
    /// @return whether the operation was successful.
    bool forceGlobal(int ks, const ModeName& mode);
    
    // switch to tonal mode for the conmputation of Weber distance.
    // @warning this array must not be closed.
    // @todo TBR
    // void setTonal();

    // switch to modal mode for the computation of Weber distance.
    // @warning this array must not be closed.
    // @todo TBR
    // void setModal();
    
private: // convenience for sub array of tonalities that can be global.
    
    /// declare that the tonality at the given index can be global.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    void setGlobal(size_t i);

    /// declare that the tonality at the given index cannot be global.
    /// @param i an index in this array of tonalities.
    /// must be smaller than size().
    void unsetGlobal(size_t i);

public: // Weber distances
    
    /// distance between the two tons at given indices,
    /// in the table of Weber, or WeberModal, or WeberBluesModal,
    /// according to the content of this array of tonalities.
    unsigned int distWeber(size_t i, size_t j) const;

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
    std::vector<std::pair<Ton, bool>> _tons;

    /// one undef ton.
    Ton _undef;
    
    /// this ton index has been closed:
    bool _closed;

    /// vector of index of representative of each ton,
    /// in the class of tonal equivalent tons.
    std::vector<size_t> _repr_tonal;

    /// vector of index of representative of each ton,
    /// in the class of modal equivalent tons.
    std::vector<size_t> _repr_modal;

    // true if we use the tonal (original) Weber distance,
    // false if we use the modal Weber distance.
    // @todo TBR not used, replaced by _WeberModal and _WeberBlues
    // bool _WeberTonal;

    /// this array of tonalities contains at least one ton with chruch mode.
    bool _WeberModal;

    /// this array of tonalities contains at least one ton with a blues mode.
    bool _WeberBluesModal;
      
    /// ranks of ton wrt Weber distance:
    /// _rankWeber[i, j] is the rank of ton j in this index, wrt to
    /// to the distance to ton i in this index.
    /// tabulation for speedup
    std::vector<std::vector<size_t>> _rankWeber;
    
    /// for initial sorting of this ton index.
    /// compare tons, ignore global flag.
    struct Ordering_operator
    {
        TonIndex* container;
        Ton base;
        Ordering_operator(TonIndex* id, int ks, ModeName mode);
        bool operator() (const std::pair<const Ton, bool>& lhs,
                         const std::pair<const Ton, bool>& rhs);
    };
    
    Ordering_operator _ordering;

    /// used to restore globals in initial state (before selection of globals)
    std::vector<bool> _backup_globals;
    
private: // construction and convenience
    
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

    /// add to the array of tonalities the tons with ks between the given
    /// bounds, and of given mode.
    /// @param ksmin lower bound (included) of ks added.
    /// @param ksmax upper bound (included) of ks added.
    /// @param mode must not be ModeName::Undef.
    /// @param global whether ton can be considered as a global tonality.
    void init(int ksmin, int ksmax, const ModeName& mode, bool global);

    // default array of 13 tonalities, ks between -6 and 6.
    // @param mode must not be ModeName::Undef.
    // @param global whether ton can be considered as a global tonality.
    // void init13(const ModeName& mode, bool global);

    // default array of 15 tonalities, ks between -7 and 7.
    // @param mode must not be ModeName::Undef.
    // @param global whether ton can be considered as a global tonality.
    // void init15(const ModeName& mode, bool global);
    
    /// array of tonalities of Bach's Wohltemperierte Clavier.
    /// major          KS -4 to 7 : C, C#, D, Eb, E, F, F#, G, Ab, A, Bb, B
    /// minor harmonic KS -6 to 6 : C, C#, D, Eb, D#, E, F, F#, G, G#, A, Bb, B
    void init25();
    
    /// not used
    void initmodal();
    
    /// initialize the structs storing representative of equivalent tons.
    void initRepr();

    /// initialize the table _rankWeber of rank wrt Weber distance.
    void initRankWeber();
    
    /// a ton in the given mode can be considered global.
    /// for automatically constructed arrays.
    /// @todo not used
    static bool global(const ModeName& mode);


    /// distance between two tons, using the Weber distance corresponding
    /// to this ton index
    /// (table of Weber, or WeberModal, or WeberBluesModal).
    unsigned int distWeber(const Ton& lhs, const Ton& rhs) const;
    
    
    /// arbitrary ordering between tons, for sorting the ton index.
    static bool ordering(const std::pair<const Ton, bool>& lhs,
                         const std::pair<const Ton, bool>& rhs);
    
    
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
