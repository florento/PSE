//
//  PSGrid.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/05/2023.
//

#include <sstream>      // std::stringbuf
#include <string>

#include "PSGrid.hpp"
#include "PSTable.hpp"


namespace pse {

PSG::PSG(const PST& tab): // std::vector<bool> mask
_index(tab.index()),
_content()
{
    // assert(mask.size() == _index.size());
    // init(tab, mask, true); // true for using the ranks
}


// not used
//PSG::PSG(const PST& tab):
//PSG(tab, std::vector<bool>(tab.index().size(), true))
//{ }


PSG::~PSG()
{ }


size_t PSG::size() const
{
    return _content.size();
}


// accessor
size_t PSG::ilocal(size_t i, size_t j) const
{
    assert(j < _content.size());
    const std::vector<size_t>& col = _content.at(j);
    assert(i < col.size());
    return col.at(i);
}


// accessor
const Ton& PSG::local(size_t i, size_t j) const
{
    size_t it = ilocal(i, j);
    assert(it < _index.size());
    return _index.ton(i);
}


// accessor
const std::vector<size_t>& PSG::column(size_t j) const
{
    assert(j < _content.size());
    const std::vector<size_t>& col = _content.at(j);
    assert(col.size() == _index.size());
    return col;
}


void PSG::init_empty(const PST& tab)
{
    size_t rows = tab.index().size();
    
    // for each bar (column)
    for (size_t j = 0; j < tab.size(); ++j)
    {
        _content.emplace_back(rows, TonIndex::UNDEF);
    }
}


// static
std::string tostring(const GridAlgo& m)
{
    switch (m)
    {
        case GridAlgo::Undef:
            return "Undef";
            
        case GridAlgo::Best:
            return "Best";
            
        case GridAlgo::Rank:
            return "Rank";
            
        case GridAlgo::Exhaustive:
            return "Exhaustive";
            
        default:
        {
            ERROR("unexpected Grid algorithm");
            return "ERROR";
        }
    }
}
            

std::ostream& operator<<(std::ostream& o, const GridAlgo& mode)
{
    o << tostring(mode);
    return o;
}


} // end namespace pse
