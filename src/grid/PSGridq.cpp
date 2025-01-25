//
//  PSGride.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/01/2025.
//

#include "PSGridq.hpp"

namespace pse {


GridState::GridState(size_t i, size_t j, size_t c):
ton(i),
measure(j),
cost(c),
previous(nullptr) // initial state
{
    DEBUG("new initial state: {}, {} ({})", ton, measure, cost);
}


GridState::GridState(std::shared_ptr<const GridState> s,
                     size_t i, size_t costplus):
ton(i),
measure(s->measure + 1),
cost(s->cost + costplus),
previous(s) // successor of s in best path. shared ptr copy.
{
    assert(s);
    DEBUG("new state: {}, {} ({})", ton, measure, cost);
}


GridState::GridState(const GridState& rhs):
ton(rhs.ton),
measure(rhs.measure),
cost(rhs.cost),
previous(rhs.previous)
{ }


GridState::~GridState()
{ }


GridState& GridState::operator=(const GridState& rhs)
{
    if (this != &rhs)
    {
        ton = rhs.ton;
        measure = rhs.measure;
        cost = rhs.cost;
        previous = rhs.previous;
    }
    return *this;
}


bool GridState::operator==(const GridState& rhs) const
{
    return (this->cost == rhs.cost and this->measure == rhs.measure);
}


bool GridState::operator!=(const GridState& rhs) const
{
    return !operator==(rhs);
}


// best (for queue top) is min cost, max measure nb.
bool GridState::operator<(const GridState& rhs) const
{
    if (this->cost == rhs.cost)
    {
        // priority to states more advanved in score
        if (this->measure == rhs.measure)
            return this->ton < rhs.ton;
        else
            return this->measure < rhs.measure;
    }
    else
        return (this->cost > rhs.cost);
}


bool GridState::operator<=(const GridState& rhs) const
{
    return !operator>(rhs);
}


bool GridState::operator>(const GridState& rhs) const
{
    return rhs.operator<(*this);
}


bool GridState::operator>=(const GridState& rhs) const
{
    return !operator<(rhs);
}


// best (for queue top) is min cost, max measure nb.
bool PSGSOrder::operator()(std::shared_ptr<const GridState>& lhs,
                           std::shared_ptr<const GridState>& rhs)
{
    assert (lhs);
    assert (rhs);
    return lhs->operator<(*rhs);
    // if (lhs->cost == rhs->cost) // priority to states more advanved in score
    // return (lhs->measure < rhs->measure);
    // else
    // return (lhs->cost > rhs->cost);  // smallest cost
};


PSGe::PSGe(const PST& tab, bool modal):
PSG(tab)
{
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
    
    init_empty(tab); // fill with TonIndex::UNDEF. can be modified emplace
    
    if (modal)
    {
        TRACE("PSGride: compute 1-row modal grid");
        init_modal(tab);
    }
    else
    {
        TRACE("PSGride: computing grid for global tons");
        for (size_t ig = 0; ig < _index.size(); ++ig)
        {
            if (_index.isGlobal(ig))
            {
                // DEBUG("PSGride: computing grid row {} ({})", ig, _index.ton(ig));
                init(tab, ig);
            }
        }
    }
}


PSGe::~PSGe()
{ }


void PSGe::init(const PST& tab, size_t ig)
{
    // priority queue for best-path computation
    PSGQueue q = PSGQueue(PSGSOrder()); // empty

    // marking visited nodes during best-path computattion
    std::vector<std::vector<bool>> visited;
    size_t columns = tab.size(); // number of measures
    for (size_t i = 0; i < _index.size(); ++i)
        visited.emplace_back(columns, false);

    // best path ties
    std::vector<std::shared_ptr<const GridState>> ties;

    // tabulation of ranks of tons / measure from the cost of tab
    std::vector<std::vector<size_t>> ranks;
    for (size_t j = 0; j < tab.size(); ++j)
    {
        const PSV& vec = tab.column(j);
        assert(vec.size() == _index.size());
        // empty bars: no rank to compute
        if (vec.first() == vec.stop())
        {
            ranks.emplace_back(); // empty column
        }
        else
        {
            ranks.emplace_back();
            assert(ranks.back().empty());
            vec.ranks(ranks.back());
            assert(ranks.back().size() == _index.size());
        }
    }

    DEBUG(".....initilization of priority queue");
    if (ig == TonIndex::UNDEF) // modal case
    {
        init_queue(q, tab, ranks);
    }
    else
    {
        assert(ig < _index.size());
        init_queue(q, tab, ranks, ig);
    }
    DEBUG(".....loop");
    // loop(q, tab, ties);
    bool terminated = false;
    
    while (!q.empty() and !terminated)
    {
        terminated = step(q, tab, ranks, visited, ties);
    }

    DEBUG(".....end");
    if (ties.empty())
    {
        ERROR("PSGride: no best path found ({})",
              (ig == TonIndex::UNDEF?
                      std::string("ton "+std::to_string(ig)):
                      std::string("modal")));
        return;
    }

    if (ties.size() > 1)
    {
        if (ig == TonIndex::UNDEF) // modal
            WARN("PSGride (modal): {} best paths", ties.size());
        else
            WARN("PSGride row {} ({}): {} best paths",
                 ig, _index.ton(ig), ties.size());
    }
    
    // modal: fill the first row
    // @todo other ton ?
    if (ig == TonIndex::UNDEF)
        ig = 0;

    // tie break: select first best path added
    // @todo it is arbitrary
    std::shared_ptr<const GridState> current = ties.front();
    assert(current);
    
    while (current != nullptr)
    {
        _content[current->measure][ig] = current->ton;
        current = current->previous;
    }
    
    // will delete all states
    ties.clear();
    while (!q.empty())
        q.pop();
    // clearVisited(visited);
}


void PSGe::init_modal(const PST& tab)
{
    init(tab, TonIndex::UNDEF);
}


// modal case
void PSGe::init_queue(PSGQueue& q, const PST& tab,
                      std::vector<std::vector<size_t>>& ranks)
{
    assert(tab.size() > 0);
    assert(ranks.size() == tab.size());
    assert(q.empty());
        
    // search for the first non-empty measure (column nb j)
    for (size_t j = 0; j < tab.size(); ++j)
    {
        const PSV& vec = tab.column(j);
        assert(vec.size() == _index.size());

        // skip empty bars
        // these columns of the grid will remain TonIndex::UNDEF
        if (vec.first() == vec.stop())
            continue;
        
        // rank of each bag in the column j (one for each ton)
        assert(j < ranks.size());
        std::vector<size_t>& rank_bags(ranks.at(j));
        assert(rank_bags.size() == _index.size());
        for (size_t i = 0; i < rank_bags.size(); ++i)
        {
            // construct an initial  std::shared_ptr<const GridState>
            // with cost = rank
            q.emplace(new GridState(i, j, rank_bags.at(i)));
        }
    }
}


// tonal case: start from global ton gton
void PSGe::init_queue(PSGQueue& q, const PST& tab,
                      std::vector<std::vector<size_t>>& ranks, size_t ig)
{
    assert(tab.size() > 0);
    assert(ranks.size() == tab.size());
    assert(q.empty());
    std::shared_ptr<const GridState> pred = nullptr;

    // first measure
    const PSV& vec = tab.column(0);
    assert(vec.size() == _index.size());

    if (vec.first() == vec.stop())
    {
        q.emplace(new GridState(ig, 0, 0)); // cost 0
    }
    else
    {
        // rank of each bag in the column 0 (one for each ton)
        std::vector<size_t>& rank_bags(ranks.at(0));
        assert(rank_bags.size() == _index.size());
        for (size_t i = 0; i < rank_bags.size(); ++i)
        {
            // construct a std::shared_ptr<const GridState>
            // with measure = 0, cost = rank + dist to ig
            q.emplace(new GridState(i, 0, _index.rankWeber(ig, i)
                                          + rank_bags.at(i)));
        }
    }
}


bool PSGe::step(PSGQueue& q, const PST& tab,
                std::vector<std::vector<size_t>>& ranks,
                std::vector<std::vector<bool>>& visited,
                std::vector<std::shared_ptr<const GridState>>& ties)
{
    assert(tab.size() > 0);
    assert(ranks.size() == tab.size());
    if (q.empty())
    {
        return true;
    }

    // extract the minimal state from the queue
    std::shared_ptr<const GridState> current = q.top();
    assert(current);
    q.pop();
    // assert(current->ton < visited.size());
    // assert(current->measure < visited.at(current->ton).size());
    visited[current->ton][current->measure] = true;
    DEBUG("Gride pop: {}, {} ({})",
          current->ton, current->measure, current->cost);

    if (!ties.empty())
    {
        assert(ties.back());
        assert(current->cost >= ties.back()->cost);
        if (current->cost > ties.back()->cost)
            return true;
    }
    
    // current in the last bar:
    // current is the last state of a path.
    size_t bar = current->measure;
    assert(tab.size() > 0);
    assert(bar < tab.size());
    if (bar == tab.size() - 1)
    {
        last(current, tab, ties);
        return false;
    }

    // compute successors of current state in next bar
    // skip empty bars (column nb j)
    for (size_t j = bar+1; j < tab.size(); ++j)
    {
        const PSV& vec = tab.column(j);
        assert(vec.size() == _index.size());
    
        // skip empty bars (making a trival path)
        if (vec.first() == vec.stop())
        {
            // state for bar j.
            // it is strictly smaller than current.
            current =
            std::make_shared<const GridState>(current,
                                              current->ton, // same local
                                              0);           // same cost
            assert(current->measure == j);

            // current (empty) bar is the last bar.
            if (j == tab.size() - 1)
            {
                if (!visited[current->ton][current->measure])
                    q.push(current);
                return false;
            }
            // else
            //    continue;
            // return false;
        }
        else
        {
            // rank of each bag in the column j (one for each ton)
            assert(j < ranks.size());
            // const std::vector<size_t>& rank_bags(ranks.at(j));
            assert(ranks.at(j).size() == _index.size());
            size_t i0 = current->ton;
            for (size_t i = 0; i < ranks.at(j).size(); ++i)
            {
                // construct a std::shared_ptr<const GridState>
                // with cost = rank
                assert(current->measure + 1 == j);
                if (!visited[i][j])
                {
                    q.emplace(new GridState(current, i,
                                            _index.rankWeber(i0, i)
                                            + ranks.at(j).at(i)));
                }
            }
            return false;
        }
    }
    return false; // never executed
}


void PSGe::last(std::shared_ptr<const GridState>& current,
                const PST& tab,
                std::vector<std::shared_ptr<const GridState>>& ties)
{
    assert(tab.size() > 0);
    assert(current->measure == tab.size() - 1);

    if (ties.empty())
    {
        ties.push_back(current);
    }
    else
    {
        assert(ties.back());
        assert(current->cost >= ties.back()->cost);
        if (current->cost == ties.back()->cost)
            ties.push_back(current);
        // otherwise, current is the last state of a non-best path, ignore.
    }
}


// static
void PSGe::clearVisited(std::vector<std::vector<bool>>& visited)
{
    for (size_t i = 0; i < visited.size(); ++i)
        for (size_t j = 0; j < visited.at(i).size(); ++j)
            visited[i][j] = false;
}


//void PSGe::loop(PSGQueue& q, const PST& tab,
//          std::vector<std::shared_ptr<const GridState>>& ties)
//{
//    bool terminated = false;
//    
//    while (!q.empty() and !terminated)
//    {
//        terminated = step(q, tab, ties);
//    }
//}


} // end namespace pse
