//
//  PS14.cpp
//  pse
//
//  Created by Florent Jacquemard on 31/05/2023.
//

#include "PS14.hpp"
//#include "CostA.hpp"
#include "CostADplus.hpp"
#include "CostADlex.hpp"


namespace pse {


PS14::PS14(size_t nbTons, bool dflag):
Speller2Pass(Algo::PSD, nbTons, dflag)
{ }


PS14::~PS14()
{
    TRACE("delete PS14");
}


//Algo PS14::algo() const
//{
//    return Algo::PS14;
//}


bool PS14::spell()
{
    //DEBUGU("Speller respell: nb tonalities in table: {}", _table.nbTons());
    if (_index.size() == 0)
    {
        /// reset to default
        /// @todo mv to speller cstr?
        WARN("Speller respell: no tonality added, use default 30 tonality array");
        for (int ks = -7; ks <= 7; ++ks)
            _index.add(ks, ModeName::Major);
        for (int ks = -7; ks <= 7; ++ks)
            _index.add(ks, ModeName::Minor);
    }
    
    /// @todo replace by other specialized costs
    CostADlex  seed0; // zero
    CostADplus seed1; // zero
    // diff0=0, diff1=0, rewrite_flag1=false
    return Speller2Pass::spell(seed0, seed1, 0, 0, false);
}




} // namespace pse
