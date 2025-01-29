//
//  debugpse.cpp
//  pypse
//
//  Created by Florent Jacquemard on 25/11/2022.
//

#include <iostream>

#include "pstrace.hpp"`
#include "PSE.hpp"
#include "PS13.hpp"
#include "PS14.hpp"
#include "Weber.hpp"
#include "WeberModal.hpp"
#include "samples.hpp"


using namespace pse;


// dump the matric of Weber distance between tonalities
//void WeberTable()
//{
//    pse::Weber godfried;
//    godfried.dump();
//}


int spellKE(SpellerEnum& sp,
            enum CostType c1, bool tonal1, bool det1,
            enum CostType c2, bool tonal2, bool det2,
            int global1,
            const GridAlgo& grid)
{
    // construct the first spelling table
    if (c1 == CostType::UNDEF)
    {
        ERROR("spellKE: cannot build first table (undef cost type)");
        return KeyFifth::UNDEF;
    }

    // spelling algo in two steps
    // clear the list of notes to spell
    // _speller.reset(0);

    // step1: construct the first spelling table
    DEBUG("spellKE: eval Table1 {} {} {}", c1, tonal1, det1);
    bool fstatus = sp.evalTable(c1, tonal1, det1);
    if (fstatus == false)
    {
        ERROR("spellKE: failed to compute Table1");
        return KeyFifth::UNDEF;
    }
    sp.printTable(std::cout);
    
    // compute the subarray of tons selected as candidate global tonality
    // tolerance distance 5%
    assert(0 <= global1);
    assert(global1 <= 100);
    bool mask = (global1 < 100);
    if (mask)
    {
        DEBUG("spellKE: eval Globals {}% approx.", global1);
        fstatus = sp.selectGlobals(global1, false);
        if (fstatus == false)
        {
            ERROR("spellKE: failed to evaluate global tonalities");
            return KeyFifth::UNDEF;
        }
        size_t nbg = sp.globals();
        DEBUG("spellKE: {} Globals selected", nbg);
    }
    
    // construct the grid of local tonalities
    if (c2 != pse::CostType::UNDEF)
    {
        DEBUG("spellKE: compute Grid with {}", grid);
        fstatus = sp.evalGrid(grid);
        if (fstatus == false)
        {
            ERROR("spellKE: failed to evaluate Grid of local tonalities");
            return KeyFifth::UNDEF;
        }
        sp.printGrid(std::cout);

    // step2: reconstruct the spelling table using the grid of local tonalities
        DEBUG("spellKE: reval Table2 {} {} {}", c2, tonal2, det2);
        fstatus = sp.revalTable(c2, tonal2, det2);
        if (fstatus == false)
        {
            ERROR("spellKE: failed to build Table2");
            return KeyFifth::UNDEF;
        }
        sp.printTable(std::cout);

    }
    
    // extract the estimated global ton from speller
    // assert(0 <= global2);
    // assert(global2 <= 100);
    DEBUG("spellKE: evaluation final list of Global tonalities");
    fstatus = sp.selectGlobals(0, true);
    if (fstatus == false)
    {
        ERROR("spellKE: failed to evaluate  final list of Global tonalities");
        return KeyFifth::UNDEF;
    }

    // extract estimated global ton
    size_t nbg = sp.globals();
    if (nbg == 0)
    {
        ERROR("spellKE: zero global ton selected");
        return KeyFifth::UNDEF;
    }
    else if (nbg > 1)
    {
        WARN("spellKE: {} ties global tons in final selection:", nbg);
        for (size_t j = 0; j < nbg; ++j)
            WARN("         global ton {}/{}: {} ({})",
                 j, nbg, sp.global(j), sp.iglobal(j));
        DEBUG("spellKE: selecting unique Global");
        if (fstatus == false)
        {
            ERROR("spellKE: failed to break ties for {} Global tonalities",
                  nbg);
            return KeyFifth::UNDEF;
        }
    }
    nbg = sp.globals();
    assert(nbg == 1);
    // index of best estimation of global ton in the array of tons of speller.
    size_t ig = sp.iglobal(0);
    assert(ig != TonIndex::UNDEF);
    const Ton& gton(sp.global(0));
    assert(!gton.undef());
    DEBUG("spellKE: best ig={}, ton={} ({})", ig, gton, gton.fifths());
    
    // rename input notes, using the spelling table computed
    DEBUG("spellKE: renaming");
    assert(ig < sp.nbTons());
    sp.rename(ig);

    // rewrite passing notes
    DEBUG("spellKE: rewriting");
    sp.rewritePassing();
      
    return sp.ton(ig).fifths();
}



int main(int argc, const char* argv[])
{
    std::cout << "Debug PS" << std::endl;
    
    setVerbosityLevel(5);
    spdlog_setPattern();

    // pse::WeberModal godfried;
    // godfried.dump();
    // return 0;

    pse::SpellerEnum sp(165, pse::Algo::Undef, true); // debug flag
    //pse::PS13 sp;

    for (size_t i = 0; i < sp.index().size(); ++i)
    {
        const Ton& ton(sp.index().ton(i));
        DEBUG("ton {}: {} {}", // (mode cast: {}={})
              i, ton,
              //ton.getMode(), static_cast<int>(ton.getMode()),
              (sp.index().isGlobal(i)?"(global)":""));
    }
    
    // feed the speller sp with notes in sample.cpp
    // issue7(sp);
    // BWV_858prelude(sp);
    // BWV_857fugue(sp);
    // BWV_864fugue(sp);
    // LG101(sp);
    // LG461(sp);
    // Waldstein(sp);
    Airegin(sp);
    // Afternoon(sp);

    std::cout << "spelling " << sp.size() << " notes" << std::endl;
    int ks = spellKE(sp,
                  // cost                   modal  deterministic
                     pse::CostType::ACCID,  false, true,
                  // cost                   tonal  deterministic
                     pse::CostType::ADplus, true, true,
                  // global1 grid algo
                     100 ,      GridAlgo::Exhaustive);

    if (ks == KeyFifth::UNDEF)
    {
        ERROR("spell failure");
        return 1;
    }

    // std::cout << "renaming " << sp.size() << " notes" << std::endl;
    // status = status && sp.rename();
//    if (status)
//    {
//        std::cout << sp.size() << " spelled notes" << std::endl;
//        for (size_t i = 0; i < sp.size(); ++i)
//        {
//            std::cout << i << ' ';
//            std::cout << sp.name(i);
//            std::cout << sp.octave(i);
//            std::cout << sp.accidental(i);
//            std::cout << std::endl;
//        }
//    }
//    else 
//    {
//        ERROR("renaming failure");
//        return 2;
//    }

//    std::cout << "rewriting" << sp.size() << " notes" << std::endl;
//    size_t rew = sp.rewritePassing();
//    std::cout << "(" << rew << " notes rewritten)" << std::endl;
    
    return 0;
}
