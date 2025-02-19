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
            ERROR("cost unique_zero: UNDEF Cost Type");
            return nullptr;

        case CostType::ACCID:
            return std::unique_ptr<Cost>(new CostA(false));;

        case CostType::ACCIDa:
            return std::unique_ptr<Cost>(new CostA(true));;

        case CostType::ACCIDtb:
            return std::unique_ptr<Cost>(new CostAT(false, false));;

        case CostType::ACCIDtba:
            return std::unique_ptr<Cost>(new CostAT(true, false));;

        case CostType::ACCIDtbs:
            return std::unique_ptr<Cost>(new CostAT(false, true));;

        case CostType::ACCIDtbas:
            return std::unique_ptr<Cost>(new CostAT(true, true));;

        case CostType::ACCIDlead:
        {
            WARN("Cost Type ACCIDlead obsolete, using ACCID");
            return std::unique_ptr<Cost>(new CostA());;
        }

        case CostType::ADplus:
            return std::unique_ptr<Cost>(new CostADplus(false, false));;

        case CostType::ADplusa:
            return std::unique_ptr<Cost>(new CostADplus(true, false));;

        case CostType::ADpluss:
            return std::unique_ptr<Cost>(new CostADplus(false, true));;

        case CostType::ADplusas:
            return std::unique_ptr<Cost>(new CostADplus(true, true));;

        case CostType::ADlex:
            return std::unique_ptr<Cost>(new CostADlex(false, false));;

        case CostType::ADlexa:
            return std::unique_ptr<Cost>(new CostADlex(true, false));;

        case CostType::ADlexs:
            return std::unique_ptr<Cost>(new CostADlex(false, true));;

        case CostType::ADlexas:
            return std::unique_ptr<Cost>(new CostADlex(true, true));;

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

        case CostType::ACCIDa:
            return "CostA_app";

        case CostType::ACCIDtb:
            return "CostAtiebreak";

        case CostType::ACCIDtba:
            return "CostAtiebreak_app";

        case CostType::ACCIDtbs:
            return "CostAtiebreak_sum";

        case CostType::ACCIDtbas:
            return "CostAtiebreak_appsum";

        case CostType::ACCIDlead:
            return "CostAdiscount";

        case CostType::ADplus:
            return "CostADplus";

        case CostType::ADplusa:
            return "CostADplus_app";

        case CostType::ADpluss:
            return "CostADplus_sum";

        case CostType::ADplusas:
            return "CostADplus_appsum";

        case CostType::ADlex:
            return "CostADlex";

        case CostType::ADlexa:
            return "CostADlex_app";

        case CostType::ADlexs:
            return "CostADlex_sum";

        case CostType::ADlexas:
            return "CostADlex_appsum";

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
