//
//  PS14.cpp
//  pse
//
//  Created by Florent Jacquemard on 31/05/2023.
//

#include "PS14.hpp"
#include "CostA.hpp" /// @todo replace by other specialized cost


namespace pse {


PS14::PS14(size_t nbTons, bool dflag):
Speller2Pass(Algo::PS14, nbTons, dflag)
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
    CostA seed0; // zero
    CostA seed1; // zero
    return Speller2Pass::spell(seed0, seed1);
}




} // namespace pse
