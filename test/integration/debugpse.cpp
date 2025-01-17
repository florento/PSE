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
            bool mask, int global1, int global2)
{
    //clear the list of notes to spell
    // _speller.reset(0);
    
    // construct the first spelling table
    if (c1 == CostType::UNDEF)
    {
        ERROR("spellKE: cannot build first table (undef cost type)");
        return KeyFifth::UNDEF;
    }
    bool fstatus = sp.evalTable(c1, tonal1, det1);
    if (fstatus == false)
    {
        ERROR("spellKE: failed to build first table");
        return KeyFifth::UNDEF;
    }
    
    // algo in two steps
    // reconstruct the spelling table using the grid of local tonalities
    if (c2 != pse::CostType::UNDEF)
    {
        // compute the subarray of tons selected as candidate global tonality
        // tolerance distance 5%
        if (mask)
        {
            assert(0 <= global1);
            assert(global1 <= 100);
            fstatus = sp.evalGlobal(global1, false);
            if (fstatus == false)
            {
                ERROR("spellKE: failed to evaluate global tonalities");
                return KeyFifth::UNDEF;
            }
        }
        
        // construct the grid of local tonalities
        fstatus = sp.evalGrid();
        if (fstatus == false)
        {
            ERROR("spellKE: failed to evaluate grid of local tonalities");
            return KeyFifth::UNDEF;
        }
        
        fstatus = sp.revalTable(c2, tonal2, det2);
        if (fstatus == false)
        {
            ERROR("spellKE: failed to build second table");
            return KeyFifth::UNDEF;
        }
    }
    
    // compute the subarray of tons selected as candidate global tonality
    // tolerance distance global2%
    assert(0 <= global2);
    assert(global2 <= 100);
    fstatus = sp.evalGlobal(global2, mask);
    if (fstatus == false)
    {
        ERROR("spellKE: failed to evaluate global tonalities");
        return KeyFifth::UNDEF;
    }

    // extract estimated global ton
    size_t nbg = sp.globals();
    if (nbg == 0)
    {
        ERROR("spellKE: could not evaluate a global ton");
        return KeyFifth::UNDEF;
    }
    else if (nbg > 1)
    {
        WARN("spellKE: {} tie estimated global tons:", nbg);
        for (size_t j = 0; j < nbg; ++j)
            WARN("estimated global ton {}: {} ({})",
                 j, sp.global(j), sp.iglobal(j));
    }
    
    // index of best estimation of global ton in the array of tons of speller.
    size_t ig = sp.iglobal(0);
    DEBUG("spellKE: best ig={}, ton={} ({})",
          ig, sp.ton(ig), sp.ton(ig).fifths());
    
    // rename input notes, using the spelling table computed
    assert(ig < sp.nbTons());
    sp.rename(ig);

    // rewrite passing notes
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

    pse::SpellerEnum sp(30, pse::Algo::Undef, true); // debug flag
    //pse::PS13 sp;

    // issue7(sp);
    // BWV_858prelude(sp);
    // BWV_857fugue(sp);
    // BWV_864fugue(sp);
    // LG101(sp);
    // LG461(sp);
    // Waldstein(sp);
    Airegin(sp);

    std::cout << "spelling " << sp.size() << " notes" << std::endl;
    int ks = spellKE(sp,
                  //                        modal  deterministic
                     pse::CostType::ACCID,  false, true,
                  //                        tonal  deterministic
                     pse::CostType::ADplus, true, true,
                  // mask   global1 global2
                     true, 5,      5);

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
