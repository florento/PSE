//
//  PSGridy.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/01/2025.
//

#include "PSGridy.hpp"

namespace pse {


PSGy::PSGy(const PST& tab, bool initialize): // std::vector<bool> mask:
PSG(tab)
{
    // assert(mask.size() == _index.size());
    assert(_content.empty());
    if (initialize)
        init(tab);
}


PSGy::~PSGy()
{ }


// selection of best local by best cost (and tie breaking)
void PSGy::init(const PST& tab)
{
    // for each bar (column)
    for (size_t j = 0; j < tab.size(); ++j)
    {
        // column of best spelling paths for measure j
        const PSV& vec = tab.column(j);
        assert(vec.size() == _index.size());

        // set of index of elements in vec with a best cost.
        // (there are several in case of tie).
        std::vector<size_t> cands; // empty
        
        extractBests(vec, cands, 50);
        // cands empty in case of empty measure
        
        // add empty column to the grid
        assert(_content.size() == j); // current nb of columns
        _content.emplace_back();
        std::vector<size_t>& current = _content.back();

        // compute every row in this new column of grid
        for (size_t i = 0; i < vec.size(); ++i)
        {
            // i is masked (not global) : do not estimate local for i
            if (!_index.isGlobal(i)) // (mask[i] == false) //
            {
                current.push_back(TonIndex::UNDEF);
            }
            // in the first column (first measure)
            else if (_content.size() == 1)
            {
                // estimation locals by extractbests then distances
                current.push_back(breakTieBest(vec, cands, i, i));
            }
            // otherwise, consider previous column
            else
            {
                assert(j > 0);
                assert(j-1 < _content.size());
                assert(i < _content.at(j-1).size());
                size_t iprev = _content.at(j-1).at(i);
                // estimation locals by extractbests then distances
                // global = i
                    current.push_back(breakTieBest(vec, cands, i, iprev));
            }
            assert(current.back() == TonIndex::UNDEF ||
                   current.back() < _index.size());
        }
        assert(current.size() == vec.size());
    }
}


// this function determines the best local tonalities
// according to the bags of the studied measure (column)
// However it has an important flaw : no tonal context is taken into account
// to determine the local tones.
void PSGy::extractBests(const PSV& vec, std::vector<size_t>& ties, double d)
{
    assert(ties.empty());
    vec.bests(ties, d); // push the bests in ties
}


// this function chooses among the best possible local tonalities
// (obtained with extract_bests)
// the closest one to the previous local tone and to the global tone.
size_t PSGy::breakTieBest(const PSV& vec, const std::vector<size_t>& cands,
                         size_t ig, size_t iprev)
{
    // no candidates in case of empty bar: keep the previous local
    if (cands.empty())
        return iprev;  // TonIndex::FAILED; // TonIndex::UNDEF
 
    assert(iprev != TonIndex::UNDEF);
    assert(iprev != TonIndex::FAILED);
    assert(iprev < _index.size());

    // index of the current best local tonality.
    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.

    for (size_t j : cands)
    {
        const PSB& psb = vec.bag(j);
        
        // occurs iff first() == last(), and in this case all the bags are empty.
        if (psb.empty()) break; // break
        
        const Cost& cost = psb.cost();
        
        // first j
        if (ibest == TonIndex::UNDEF)
        {
            ibest = j;
        }
        else
        {
            const Cost& bestcost = vec.bag(ibest).cost();
            if (cost < bestcost)
            {
                ibest = j;
            }
            else if (cost == bestcost)
            {
                // tie break criteria 1:
                // best distance (of current tonality j)
                // to the previous local tonality for ig
                // dist = pton.distWeber(jton);
                size_t rankp = _index.rankWeber(iprev, j);
                size_t bestrankp = _index.rankWeber(iprev, ibest);
                if (rankp < bestrankp)
                {
                    ibest = j; // new best
                }
                // tie break criteria 2:
                // best distance (of current tonality j)
                // to the global tonality ig
                else if (rankp == bestrankp)
                {
                    // gton.distWeber(jton);
                    size_t rankg = _index.rankWeber(ig, j);
                    size_t bestrankg = _index.rankWeber(ig, ibest);
                    if (rankg < bestrankg)
                    {
                        ibest = j; // new best
                    }
                    // tie break criteria 3:
                    // smallest key signature
                    // ALT: best distance between the previous local tonality and
                    // a config in bag for the current tonality j
                    else if (rankg == bestrankg)
                    {
                        const Ton& ton = _index.ton(j);
                        int fifths = std::abs(ton.fifths());
                        const Ton& bestton = _index.ton(ibest);
                        int bestfifth = std::abs(bestton.fifths());
                        if (fifths < bestfifth)
                        {
                            ibest = j; // new best
                        }
                        // tie break fail
                        else if (fifths == bestfifth)
                        {
                            assert(j != ibest);
                            // tie break criteria 4:
                            // Major, Minor > modes
                            if (ton.isMajorMinor() && !bestton.isMajorMinor())
                            {
                                ibest = j; // new best
                            }
                            else if (ton.isMajorMinor() and
                                     bestton.isMajorMinor())
                            {
                                // tie break criteria 5:
                                // Major > Minor
                                if ((ton.getMode() == ModeName::Major) and
                                    (bestton.getMode() == ModeName::Minor))
                                {
                                    ibest = j; // new best
                                }
                                // signal
                                // - different color (same abs)
                                // - uncomparable modes
                                else if ((ton.fifths() != bestton.fifths()) or
                                         (!ton.isMajorMinor() and
                                          !bestton.isMajorMinor()))
                                {
                                    WARN("PSGrid: tie break fail {}: {}:{} vs {}:{}",
                                         vec.bar(), j, ton, ibest, bestton);
                                    // WARN_TIE(vec.bar(), cands, ibest);
                                    // keep the former (smaller) ibest
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    assert(ibest != TonIndex::FAILED);
    assert(ibest == TonIndex::UNDEF || ibest < _index.size());
    assert(ibest < _index.size() || cands.empty());
    return ibest;
}


void PSGy::WARN_TIE(size_t barnb, const std::vector<size_t>& ties,
                   size_t ibest) const
{
    std::stringstream buffer;
    for (size_t i : ties)
        buffer << std::to_string(i) << ": "<< _index.ton(i) << "  ";
    if (ibest != TonIndex::UNDEF)
    {
        DEBUGU("estimateLocal: {} ties bar {} (selected={}): {}",
             ties.size(), barnb, ibest, buffer.str());
    }
    else
    {
        DEBUGU("estimateLocal: {} ties bar {}: {}",
             ties.size(), barnb, buffer.str());
    }
}


} // end namespace pse



/*
void PSGy::init(const PST& tab, std::vector<bool> mask)
{

    for (size_t j = 0; j < tab.measures(); ++j)
    {
        // column of best paths for measure j
        const PSV& vec = tab.column(j);
        assert(vec.size() == mask.size());

        // set of index of elements in vec with a best cost.
        // (there are several in case of tie).
        //std::vector<size_t> ties; // empty
        // cands empty in case of empty measure
        
        // add empty column
        assert(_content.size() == j); // current nb of columns
        _content.emplace_back();
        std::vector<size_t>& current = _content.back();

        // compute every row in this new column
        for (size_t i = 0; i < vec.size(); ++i)
        {
            // i is masked : do not estimate local for i
            if (mask[i] == false)
            {
                current.push_back(TonIndex::UNDEF);
            }
            // in the first column (first measure)
            else if (_content.size() == 1)
            {
                current.push_back(estimateLocalalt(vec, i, i, 4));
            }
            // otherwise, consider previous column
            else
            {
                assert(j > 0);
                assert(j-1 < _content.size());
                assert(i < _content.at(j-1).size());
                size_t iprev = _content.at(j-1).at(i);
                current.push_back(estimateLocalalt(vec, i, iprev, 4));
            }
            assert(current.back() == TonIndex::UNDEF ||
                   current.back() < _index.size());
            //INFO("BUG 102: {} (UNDEF={})", _content.back().back(), TonIndex::UNDEF);
        }
        assert(current.size() == vec.size());
    }
}
*/

/*
 // this alternative function determines the best local tonality by restraining
 // its search only on tones close to the previous or global one
 // and then choosing the one minimizing accidents
 /// #todo AV not used?
 size_t PSGy::estimateLocalLexico(const PSV& vec, size_t ig, size_t iprev,
                                 unsigned int d)
 {
     // no candidates in case of empty bar: keep the previous local
     //if (cands.empty())
     //    return iprev;  // TonIndex::FAILED; // TonIndex::UNDEF
     std::set<size_t> cands ;
     std::set<size_t> ties ;
     
     
     assert(iprev != TonIndex::UNDEF);
     assert(iprev != TonIndex::FAILED);
     assert(iprev < _index.size());
     
     const Ton& pton = _index.ton(iprev);
     const Ton& gton = _index.ton(ig);

     // index of the current best local tonality.
     size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.
     
     // current best distance to previous local ton. for ig
     //unsigned int dbest = 30; // initialized to avoid warning

     // current best distance to previous global ton. ig
     //unsigned int dgbest = 30; // initialized to avoid warning

     for (size_t j=0; j < _index.size(); ++j)
     {
         
         const Ton& jton = _index.ton(j);
         unsigned int dist = pton.distWeber(jton);
         unsigned int distg = gton.distWeber(jton);
         // éventuellement remplacer Weber par la simple distance du cycle des quintes
         if ((dist <= d) || (distg <= d))
         {
             cands.insert(j);
             //ibest = j;
             //continue;
         }
     }
     
     std::shared_ptr<Cost> cbest = nullptr;
     int cpt = 0;
     for (size_t j : cands)
     {
         const PSB& psb = vec.bag(j);
         
         // occurs iff first() == last(), and in this case all the bags are empty.
         if (psb.empty())
             break; // break
         
         const Cost& cost = psb.cost(); // shared_clone();
         
         // new best cost     // cbest = 0 when j = 0
         if ((cpt == 0) || ((cbest != nullptr) && (cost < *cbest)))
         {
             ibest = j;
             cbest = cost.shared_clone();
             cpt++;
             //ties.clear();
             //auto ret = ties.insert(j);
             //assert(ret.second == true); // j was inserted
         }
     }
     
     for (size_t j : cands)
     {
         const PSB& psb = vec.bag(j);
         
         // occurs iff first() == last(), and in this case all the bags are empty.
         if (psb.empty())
             break; // break
         
         const Cost& cost = psb.cost(); // shared_clone();
         // tie break
         if ((cbest != nullptr) && (cost == *cbest) && (j != ibest))
         {
             auto ret = ties.insert(j);
             assert(ret.second == true); // j was inserted
             const Ton& jton=_index.ton(j);
             unsigned int dist = pton.distWeber(jton);
             unsigned int distg = gton.distWeber(jton);
             WARN("PSGrid, estimation local ALT, tie break fail {} vs {} dist prev({})={}, dist global({})={})",
                  jton, _index.ton(ibest), pton, dist, gton, distg);
         }
         // otherwise keep the current best
         else
         {
             assert((cbest == nullptr) || (cost > *cbest)); // we did not forget a case
         }
     }
     assert(ibest != TonIndex::FAILED);
     assert(ibest == TonIndex::UNDEF || ibest < _index.size());
     assert(ibest < _index.size() || cands.empty());
     
     return ibest;
 }
  */
