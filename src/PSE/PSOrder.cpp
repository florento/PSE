//
//  PSOrder.hpp
//  pse
//
//  Created by Florent Jacquemard on 22/02/2023.
//
/// @addtogroup pitch
/// @{ 


#include "PSOrder.hpp"


namespace pse {


std::ostream& operator<<(std::ostream& o, const CostOrdering& co)
{
    switch (co)
    {
        case CostOrdering::Default:
            o << "Ord_Default";
            break;

        case CostOrdering::Lex:
            o << "Ord_Lex";
            break;

        case CostOrdering::Cumul:
            o << "Ord_Cumul";
            break;

        case CostOrdering::Approx:
            o << "Ord_Approx";
            break;

        case CostOrdering::Undef:
            o << "Ord_Undef";
            break;

        default:
        {
            ERROR("unknown Ton mode");
            o << "Ord_ERR";
            break;
        }
    }
    return o;
}


} // end namespace pse

/// @}

