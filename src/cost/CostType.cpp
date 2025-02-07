//
//  CostType.cpp
//  pse
//
//  Created by Florent Jacquemard on 12/04/2024.
//

#include "CostType.hpp"
#include "Cost.hpp"
#include "CostA.hpp"
#include "CostAT.hpp"
#include "CostADplus.hpp"
#include "CostADlex.hpp"


namespace pse {


std::unique_ptr<Cost> unique_zero(const CostType& ct)
{
    switch (ct)
    {
        case CostType::UNDEF:
            return nullptr;

        case CostType::ACCID:
            return std::unique_ptr<Cost>(new CostA());;

        case CostType::ACCIDtb:
            return std::unique_ptr<Cost>(new CostAT());;

        case CostType::ACCIDlead:
        {
            WARN("Cost Type ACCIDlead obsolete, using ACCID");
            return std::unique_ptr<Cost>(new CostA());;
        }

        case CostType::ADplus:
            return std::unique_ptr<Cost>(new CostADplus());;

        case CostType::ADlex:
            return std::unique_ptr<Cost>(new CostADlex());;

        default:
        {
            ERROR("unknown Cost Type");
            return nullptr;
        }
    }
}


std::string tostring(const CostType& ct)
{
    switch (ct)
    {
        case CostType::UNDEF:
            return "undef";

        case CostType::ACCID:
            return "CostA";

        case CostType::ACCIDtb:
            return "CostAtiebreak";

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
