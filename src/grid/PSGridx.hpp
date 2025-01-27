//
//  PSGridx.hpp
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
#include "PSGrid.hpp"


namespace pse {

/// Construction of a grid from a table with an exhaustive
/// Viterbi algorithm.
class PSGx : public PSG
{
    
public: // construction
    
    /// construction of grid from a table.
    /// @param tab pitch spelling table used to estimated the locals.
    /// @param modal compute only one row of the grid, of index 0,
    /// without assumed global topnality. Otherwise, compute all rows
    /// associated to global tonalities in the tone index.
    PSGx(const PST& tab, bool modal=false);

    /// a grid cannot be copied
    PSGx(const PSGx& rhs) = delete;
                  
    /// destructor
    virtual ~PSGx();

    /// a grid cannot be copied
    PSGx& operator=(const PSGx& rhs) = delete;

private: // construction
    
    /// compute one row of this grid of local tons, corresponding to
    /// the given global ton, using the given spelling table.
    /// @param tab spelling table filled with spelling costs.
    /// @param ig global ton of the index of tab, or TonIndex::UNDEF.
    /// in the latter case, compute one unique row of index 0 of this grid
    /// of local tons, for modal case (no global tonality).
    void init(const PST& tab, size_t ig);

    /// compute one unique row of index 0 of this grid of local tons,
    /// for modal case (no global tonality).
    void init_modal(const PST& tab);

    // use the default ordering > (and ==) on Grid States
    // PSGQueue q = PSGQueue(PSGlex());
    
    /// compute the first column of nodes of best paths (cells),
    /// according to the spelling costs in the first column of the given table,
    /// for a modal construction of the grid (no global tonality).
    /// @param tab spelling table filled with spelling costs.
    /// must be non empty (nb bars > 0).
    /// @param ranks precomputed rankings of each column of tab.
    /// @param costs table of best-path costs.
    /// @param preds table of of predecessors in best paths.
    /// @return the number of the next column to proceed.
    size_t first(const PST& tab,
               const std::vector<std::vector<size_t>>& ranks,
               std::vector<std::vector<size_t>>& costs,
               std::vector<std::vector<size_t>>& preds);

    /// compute the first column of nodes of best paths (cells),
    /// according to the spelling costs in the first column of the given table
    /// and the given assumed global ton.
    /// @param tab spelling table filled with spelling costs.
    /// must be non empty (nb bars > 0).
    /// @param ranks precomputed rankings of each column of tab.
    /// @param costs table of best-path costs.
    /// @param preds table of of predecessors in best paths.
    /// @return the number of the next column to proceed.
    size_t first(const PST& tab,
               const std::vector<std::vector<size_t>>& ranks,
               std::vector<std::vector<size_t>>& costs,
               std::vector<std::vector<size_t>>& preds,
               size_t ig);

    /// compute one columb of best path nodes.
    /// @param j column number.
    /// must be larger or equal to 1.
    /// must be smaller than the siae of tab (number of measures)
    /// @param tab spelling table filled with spelling costs.
    /// must be non empty (nb bars > 0).
    /// @param ranks precomputed rankings of each column of tab.
    /// @param costs table of best-path costs.
    /// must have a column j-1.
    /// @param preds table of of predecessors in best paths.
    /// must have a column j-1.
    /// @return the number of the next column to proceed.
    size_t column(size_t j, const PST& tab,
                  const std::vector<std::vector<size_t>>& ranks,
                  std::vector<std::vector<size_t>>& costs,
                  std::vector<std::vector<size_t>>& preds);

    /// index of best cost in a column.
    /// @param col on column in the table of best-path costs.
    /// @para ig current assumed global ton, for feedback.
    /// @return the index of the best cost in col,
    /// with priority to smaller index in case of tie.
    size_t bestCost(const std::vector<size_t>& col, size_t ig);
    
    
private: // static constants
    
    static size_t COST_INFINITE;

    static size_t PRED_UNDEF;
    
    // content of a cell at ton i and measure j (pimpl)
    // struct Cell
    // {
    //     public:
    //
    //         /// weight value of this node.
    //         size_t cost;
    //
    //         /// node of best path in previous column
    //         size_t previous;
    //
    //         Cell(size_t c, size_t p);
    // };

};


} // namespace pse

#endif /* PSGride_hpp */

/// @}
