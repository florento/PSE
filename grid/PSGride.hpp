//
//  PSGride.hpp
//  pse
//
//  Created by Florent Jacquemard on 24/01/2025.
//
/// @addtogroup pitch
/// @{

#ifndef PSGride_hpp
#define PSGride_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <vector>
#include <stack>
#include <queue>   // std::priority_queue

#include "pstrace.hpp"
#include "utils.hpp"
#include "PSGridy.hpp"


namespace pse {


struct GridState
{
    
public: // data

    /// index of local ton in embedding ton array (row in grid).
    size_t ton;
    
    /// measure number (column in grid).
    size_t measure;
    
    /// weight value assigned to this state.
    size_t cost;

    /// previous state in a best path.
    std::shared_ptr<const GridState> previous;
    
public: // construction
    
    /// initial grid state, of cost 0, without predecessor.
    /// @param i index of local ton of the state.
    /// @param j number of measure of the state.
    /// @param c cost of the state.
    GridState(size_t i, size_t j, size_t c = 0);

    /// successor of the given grid state.
    /// @param s predecessor of this state in a best path.
    /// the measure (column) number of this state increments the one of s.
    /// @param i index of local ton for this state.
    /// @param costplus increment for computing the cost of this state
    /// from the cost of s.
    GridState(std::shared_ptr<const GridState> s, size_t i, size_t costplus);

    /// copy cnstructor
    GridState(const GridState& rhs);
    
    ~GridState();

    GridState& operator=(const GridState& rhs);
    
public: // operator for ordering in a queue of states
    
    // equality of cost.
    bool operator==(const GridState& rhs) const;
    
    /// negation of ==.
    bool operator!=(const GridState& rhs) const;

    // strict ordering for costs.
    bool operator<(const GridState& rhs) const;

    /// negation of >.
    bool operator<=(const GridState& rhs) const;

    /// inverse (commutation) of <.
    bool operator>(const GridState& rhs) const;

    /// negation of <.
    bool operator>=(const GridState& rhs) const;
    
};

/// ordering for building a queue of PSGrid States
/// the best state (for queue top) has
/// - min cost
/// - in case of equal costs, max measure nb.
struct PSGSOrder
{
    bool operator()(std::shared_ptr<const GridState>& lhs,
                    std::shared_ptr<const GridState>& rhs);
   
};

/// type of orderings for container of PSGrid states
using PSGCompare = std::function<bool(std::shared_ptr<const GridState>&,
                                      std::shared_ptr<const GridState>&)>;

/// priority queue of PSGrid states
typedef std::priority_queue<std::shared_ptr<const GridState>,
                            std::vector<std::shared_ptr<const GridState>>,
                            PSGCompare> PSGQueue;

/// Construction of a grid from a table with an exhaustive
/// best-path search algorithm.
class PSGe : public PSG
{
    
public: // construction
    
    /// construction of grid from a table.
    /// @param tab pitch spelling table used to estimated the locals.
    /// @param modal compute only one row of the grid, of index 0,
    /// without assumed global topnality. Otherwise, compute all rows
    /// associated to global tonalities in the tone index.
    PSGe(const PST& tab, bool modal=false);

    /// a grid cannot be copied
    PSGe(const PSGe& rhs) = delete;
                  
    /// destructor
    virtual ~PSGe();

    /// a grid cannot be copied
    PSGe& operator=(const PSGe& rhs) = delete;

private: // construction
    
    /// compute one row of this grid of local tons, corresponding to
    /// the given global ton, using the given spelling table.
    /// @param tab spelling table.
    /// @param ig global ton of the index of tab, or TonIndex::UNDEF.
    /// in the latter case, compute one unique row of index 0 of this grid
    /// of local tons, for modal case (no global tonality).
    void init(const PST& tab, size_t ig);

    /// compute one unique row of index 0 of this grid of local tons,
    /// for modal case (no global tonality).
    void init_modal(const PST& tab);

    // use the default ordering > (and ==) on Grid States
    // PSGQueue q = PSGQueue(PSGlex());
    
    /// add the first grid states to the given queue,
    /// according to the spelling costs in the first column of the given table,
    /// for a modal construction of the grid (no global tonality).
    /// @warning the table must be non empty (nb bars > 0).
    /// @warning q must be empty.
    void init_queue(PSGQueue& q, const PST& tab);

    /// add the first grid states to the given queue,
    /// according to the spelling costs in the first column of the given table
    /// and the given assumed global ton.
    /// @warning the table must be non empty (nb bars > 0).
    /// @warning q must be empty.
    void init_queue(PSGQueue& q, const PST& tab, size_t ig);

private: // data
    
    /// step in the best path construction
    /// @param ties container for the complete best paths found.
    /// they all have the same cost.
    /// @return whether the computation is terminated.
    /// @warning the table must be non empty (nb bars > 0).
    bool step(PSGQueue& q, const PST& tab,
              std::vector<std::vector<bool>>& visited,
              std::vector<std::shared_ptr<const GridState>>& ties);

    /// process a final state in a best path.
    /// @param ties container for the complete best paths found.
    /// they all have the same cost.
    /// @warning the table must be non empty (nb bars > 0).
    void last(std::shared_ptr<const GridState>& current,
              const PST& tab,
              std::vector<std::shared_ptr<const GridState>>& ties);

    static void clearVisited(std::vector<std::vector<bool>>& visited);
    
    // void loop(PSGQueue& q, const PST& tab,
    //           std::vector<std::shared_ptr<const GridState>>& ties);
    
};


} // namespace pse

#endif /* PSGride_hpp */

/// @}
