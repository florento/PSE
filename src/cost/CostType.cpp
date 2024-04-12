//
//  CostType.cpp
//  pse
//
//  Created by Florent Jacquemard on 12/04/2024.
//

#include "CostType.hpp"



namespace pse {

std::string tostring(const CostType& t)
{
    switch (t)
    {
        case CostType::UNDEF:
            return "undef";

        case CostType::ACCID:
            return "CostA";

        case CostType::ACCIDlead:
            return "CostAdiscount";

        case CostType::ADplus:
            return "CostADplus";

        case CostType::ADlex:
            return "CostADlex";
            
        default:
        {
            ERROR("unknown Cost Type");
            return "ERROR";
        }
    }
}


std::ostream& operator<<(std::ostream& o, const CostType& t)
{
    o << tostring(t);
    return o;
}



} // end namespace pse
