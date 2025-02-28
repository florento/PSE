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
#include "CostADplex.hpp"


namespace pse {


std::unique_ptr<Cost> unique_zero(const CostType& ct)
{
    switch (ct)
    {
        case CostType::UNDEF:
            ERROR("cost unique_zero: UNDEF Cost Type");
            return nullptr;

        case CostType::ACCID:
            return std::unique_ptr<Cost>(new CostA());;

        case CostType::ACCIDtb:
            return std::unique_ptr<Cost>(new CostAT(true)); // lexico ordering

        case CostType::ACCIDtbs:
            return std::unique_ptr<Cost>(new CostAT(false)); // partial sum TB

        case CostType::ACCIDlead:
        {
            WARN("Cost Type ACCIDlead obsolete, using ACCID");
            return std::unique_ptr<Cost>(new CostA());;
        }

        case CostType::ADplus:
            return std::unique_ptr<Cost>(new CostADplus(true));;

        case CostType::ADpluss:
            return std::unique_ptr<Cost>(new CostADplus(false));;
            
        case CostType::ADlex:
            return std::unique_ptr<Cost>(new CostADlex(true));;

        case CostType::ADlexs:
            return std::unique_ptr<Cost>(new CostADlex(false));;

        case CostType::ADplex:
            return std::unique_ptr<Cost>(new CostADplex(true));;

        case CostType::ADplexs:
            return std::unique_ptr<Cost>(new CostADplex(false));;

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

        case CostType::ACCIDtbs:
            return "CostAtiebreak_sum";

        case CostType::ACCIDlead:
            return "CostAdiscount";

        case CostType::ADplus:
            return "CostADplus";

        case CostType::ADpluss:
            return "CostADplus_sum";

        case CostType::ADlex:
            return "CostADlex";

        case CostType::ADlexs:
            return "CostADlex_sum";

        case CostType::ADplex:
            return "CostADplex";

        case CostType::ADplexs:
            return "CostADplex_sum";

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
