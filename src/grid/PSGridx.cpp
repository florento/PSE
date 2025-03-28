//
//  PSGridx.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/01/2025.
//

#include "PSGridx.hpp"
#include "PSTable.hpp"


namespace pse {

// static
size_t PSGx::COST_INFINITE = -1;

// static
size_t PSGx::PRED_UNDEF = -1;

// static
const std::array<size_t, 3> PSGx::COEFF = {2, 1, 1};


//PSGx::Cell::Cell(size_t c, size_t p):
//cost(c),
//previous(p)
//{ }


PSGx::PSGx(const PST& tab, bool singleton):
PSG(tab)
{
    init_empty(tab);
    
    // no tons, no grid.
    if (_index.empty())
    {
        WARN("Grid computation: no tons, the grid is empty.");
        return;
    }
    
    // no notes, no grid.
    if (tab.size() == 0)
    {
        WARN("Grid computation: no measures, the grid is empty.");
        return;
    }
        
    if (singleton)
    {
        TRACE("PSGride: compute 1-row modal grid");
        init_singleton(tab);
    }
    else
    {
        TRACE("PSGride: computing grid for global tons");
        for (size_t ig = 0; ig < _index.size(); ++ig)
        {
            if (_index.isGlobal(ig))
            {
                TRACE("PSGridx: computing grid row {} ({})", ig, _index.ton(ig));
                init(tab, ig);
            }
        }
    }
}


PSGx::~PSGx()
{ }


void PSGx::init_singleton(const PST& tab)
{
    init(tab, TonIndex::UNDEF);
}


void PSGx::init(const PST& tab, size_t ig)
{
    assert(tab.size() > 0);

    // tabulation of ranks of tons / measure from the cost of tab
    std::vector<std::vector<size_t>> ranks;
    for (size_t j = 0; j < tab.size(); ++j)
    {
        const PSV& barj = tab.column(j);
        assert(barj.size() == _index.size());
        ranks.emplace_back();
        if (barj.first() == barj.stop()) // empty bars: no rank to compute
        {
            // leave empty column
        }
        else
        {
            assert(ranks.back().empty());
            barj.ranks(ranks.back());
            assert(ranks.back().size() == _index.size());
        }
    }
    assert(ranks.size() == tab.size());
    
    // tonindex * measures-table of best-path costs
    std::vector<std::vector<size_t>> costs;

    // tonindex * measures-table of predecessors in best path.
    std::vector<std::vector<size_t>> preds;
    
    // initialize with empty columns
    for (size_t j = 0; j < tab.size(); ++j)
    {
        costs.emplace_back();
        preds.emplace_back();
    }
    
    // fill the first column
    size_t j = 0;
    if (ig == TonIndex::UNDEF) // modal case
    {
        j = first(tab, ranks, costs, preds);
    }
    else
    {
        assert(ig < _index.size());
        j = first(tab, ranks,  costs, preds, ig);
    }
    
    // fill the whole table with best-path costs and preds
    for (; j < tab.size();)
    {
        j = column(j, tab, ranks, costs, preds, ig);
    }
    
    // tonal: fill the row ig
    // modal: fill the first row
    // @todo other ton ?
    if (ig == TonIndex::UNDEF)
        ig = 0;

    // extract the best path
    assert(costs.size() == tab.size());
    assert(preds.size() == tab.size());
    assert(_content.size() == tab.size());

    if (costs.back().empty() or preds.back().empty())
    {
        WARN("Gridx: failure in computation of best path");
        return;
    }
    // index of row (ton) with best (cumulated) cost in last column
    size_t i = bestCost(costs.back(), ig);
    
    for (size_t jm = 1; jm <= tab.size(); ++jm)
    {
        j = tab.size() - jm;
        assert(i < _index.size());
        assert(i != TonIndex::UNDEF);
        assert(j < _content.size());
        assert(ig < _content.at(j).size());
        _content[j][ig] = i;
        if (preds.at(j).empty())
            continue;
        else
        {
            assert(i < preds.at(j).size());
            i = preds.at(j).at(i);
        }
    }
}



// modal (singleton) case
size_t PSGx::first(const PST& tab,
                   const std::vector<std::vector<size_t>>& ranks,
                   std::vector<std::vector<size_t>>& costs,
                   std::vector<std::vector<size_t>>& preds)

{
    assert(tab.size() > 0);
    assert(ranks.size() == tab.size());
    assert(costs.size() == tab.size());
    assert(preds.size() == tab.size());
    
    // search for the first non-empty measure (column nb j)
    for (size_t j = 0; j < tab.size(); ++j)
    {
        const PSV& bar = tab.column(j);
        assert(bar.size() == _index.size());
        
        // skip empty bars
        // these columns of the grid will remain TonIndex::UNDEF
        if (bar.first() == bar.stop())
        {
            // columns in costs and preds remain empty
            assert(costs.at(j).empty());
            assert(preds.at(j).empty());
        }
        else
        {
            // rank of each bag in the column j (one for each ton)
            const std::vector<size_t>& rankj(ranks.at(j));
            assert(rankj.size() == _index.size());
            std::vector<size_t>& costj(costs.at(j));
            assert(costj.empty());
            for (size_t i = 0; i < rankj.size(); ++i)
            {
                // initial cost = rank
                costj.push_back(rankj.at(i));
            }
            // column in preds remains empty
            assert(preds.at(j).empty());
            return j+1;
        }
    }
    // should not happen (all bars empty)
    return tab.size();
}


// tonal case: start from global ton gton
size_t PSGx::first(const PST& tab,
                   const std::vector<std::vector<size_t>>& ranks,
                   std::vector<std::vector<size_t>>& costs,
                   std::vector<std::vector<size_t>>& preds, size_t ig)
{
    assert(tab.size() > 0);
    assert(ranks.size() == tab.size());
    assert(costs.size() == tab.size());
    assert(preds.size() == tab.size());

    // first measure
    const PSV& bar = tab.column(0);
    assert(bar.size() == _index.size());
    std::vector<size_t>& costj(costs.at(0));
    
    // empty bar (no spelling cost values)
    if (bar.first() == bar.stop())
    {
        for (size_t i = 0; i < _index.size(); ++i)
        {
            costj.push_back(COST_INFINITE);
        }
        assert(ig < costj.size());
        costj.at(ig) = 0;
        // column in preds remains empty
        assert(preds.at(0).empty());
    }
    else
    {
        // rank of each bag in the column 0 (one for each ton)
        const std::vector<size_t>& rankj(ranks.at(0));
        assert(rankj.size() == _index.size());
        for (size_t i = 0; i < _index.size(); ++i)
        {
            // initial cost = rank + dist to ig
            costj.push_back((COEFF[1]+COEFF[2]) * _index.rankWeber(ig, i) +
                             COEFF[0] * rankj.at(i));
        }
        // column in preds remains empty
        assert(preds.at(0).empty());
    }
    return 1;
}


size_t PSGx::column(size_t j, const PST& tab,
                    const std::vector<std::vector<size_t>>& ranks,
                    std::vector<std::vector<size_t>>& costs,
                    std::vector<std::vector<size_t>>& preds,
                    size_t ig)
{
    assert(tab.size() > 0);
    assert(ranks.size() == tab.size());
    assert(costs.size() == tab.size());
    assert(preds.size() == tab.size());
    assert(0 < j and j < tab.size());
    assert(j-1 < costs.size());
    assert(j-1 < preds.size());
    assert(ig == TonIndex::UNDEF or ig < _index.size());

    // best path costs so far (until the measure preceeding j)
    const std::vector<size_t>& pcosts = costs.at(j-1);
    assert(pcosts.size() == _index.size());
    assert(costs.at(j).empty());
    assert(preds.at(j).empty());

    // rank of each bag in the column j (one for each ton)
    const std::vector<size_t>& rankj(ranks.at(j));

    // measure j
    const PSV& barj = tab.column(j);
    assert(barj.size() == _index.size());
    bool empty_bar = (barj.first() == barj.stop());
    assert(empty_bar or rankj.size() == _index.size());
    assert(!empty_bar or rankj.empty());

    // process column j
    for (size_t i = 0; i < _index.size(); ++i)
    {
        // continue in the same local tonality
        if (empty_bar)
        {
            size_t predcost = pcosts.at(i);
            if (ig != TonIndex::UNDEF) predcost += _index.rankWeber(ig, i);
            assert(_index.rankWeber(i, i) == 0);
            costs.at(j).push_back(predcost);
            preds.at(j).push_back(i);
            continue;
        }

        // search best predecessor
        size_t best_cost = COST_INFINITE;
        size_t best_pred = PRED_UNDEF;
        size_t best_predcost = COST_INFINITE;
        // rank of spelling cost for ton i at bar j
        /// @todo rank or cost value?
        size_t ri = empty_bar?0:(COEFF[0] * rankj.at(i));
        // rank of i for distance to global
        if (ig != TonIndex::UNDEF) ri += COEFF[2] * _index.rankWeber(ig, i);
        // select a best predecessor for i
        for (size_t ip = 0; ip < _index.size(); ++ip)
        {
            if (pcosts.at(ip) == COST_INFINITE)
                continue;
            size_t predcost = pcosts.at(ip);
            /// @todo distWeber or rankWeber
            size_t cost = predcost + COEFF[1] * _index.rankWeber(ip, i) + ri;
            
            // in case of tie, we keep the smaller best_pred
            // hence we need a wise ordering of tons in the ton index.
            if (best_cost == COST_INFINITE or cost < best_cost)
            {
                best_cost = cost;
                best_pred = ip;
                best_predcost = predcost;
            }
            // tie break
            else if (cost == best_cost and
            (best_predcost == COST_INFINITE or predcost < best_predcost))
            {
                best_pred = ip;
                best_predcost = predcost;
            }
            // otherwise ignore
        }
        assert(best_cost != COST_INFINITE);
        assert(best_pred != PRED_UNDEF);
        costs.at(j).push_back(best_cost);
        preds.at(j).push_back(best_pred);
    }
    assert(costs.at(j).size() == _index.size());
    assert(preds.at(j).size() == _index.size());

    return j+1;
}


size_t PSGx::bestCost(const std::vector<size_t>& col, size_t ig)
{
    size_t best_cost = COST_INFINITE;
    size_t ibest = PRED_UNDEF;
    
    size_t ties = 0;
    for (size_t i = 0; i < col.size(); ++i)
    {
        if (best_cost == COST_INFINITE or col.at(i) < best_cost)
        {
            best_cost = col.at(i);
            ibest = i;
            ties = 1;
        }
        else if (best_cost != COST_INFINITE and col.at(i) == best_cost)
        {
            ties++;
        }
    }
    assert(best_cost != COST_INFINITE);
    assert(ibest != PRED_UNDEF);
    assert(ties > 0);
    if (ties > 1)
    {
        if (ig == TonIndex::UNDEF)
        {
            TRACE("Gridx: {} best path ties (cost {})", ties, best_cost);
        }
        else
        {
            assert(ig < _index.size());
            TRACE("Gridx {}: {} best path ties (cost {})",
                 _index.ton(ig), ties, best_cost);
        }
    }
        
    return ibest;
}


} // end namespace pse
