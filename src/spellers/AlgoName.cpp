//
//  AlgoName.cpp
//  pse
//
//  Created by Florent on 11/05/2023.
//

#include "AlgoName.hpp"

namespace pse {

// static
std::string tostring(const Algo& a)
{
    switch (a)
    {
        case Algo::Undef:
            return "undef";

        case Algo::PSE:
            return "PSE";

        // case Algo::PSE0:
        //    return "PSE0";

        //case Algo::PSE1:
        //    return "PSE1";

        case Algo::PS13:
            return "PS13";

        case Algo::PS14:
            return "PS14";

        case Algo::RewritePassing:
            return "RewritePassing";
            
        default:
        {
            ERROR("unknown Ton mode");
            return "ERROR";
        }
    }
}


std::ostream& operator<<(std::ostream& o, const Algo& a)
{
    o << tostring(a);
    return o;
}


} // end namespace pse
