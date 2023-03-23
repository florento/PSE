//
//  ModeName.cpp
//  pse
//
//  Created by Florent on 23/03/2023.
//

#include "ModeName.hpp"

namespace pse {


// static
std::string tostring(const ModeName& m)
{
    switch (m)
    {
        case ModeName::Undef:
            return "undef";

        case ModeName::Maj:
            return "maj";

        case ModeName::Min:
            return "min";

        case ModeName::MinNat:
            return "min nat";

        case ModeName::MinMel:
            return "min mel";

        case ModeName::Ionian:
            return "Ionian";

        case ModeName::Dorian:
            return "Dorian";

        case ModeName::Phrygian:
            return "Phrygian";

        case ModeName::Lydian:
            return "Lydian";

        case ModeName::Mixolydian:
            return "Mixolydian";
            
        case ModeName::Eolian:
            return "Eolian";

        case ModeName::Locrian:
            return "Locrian";

        default:
        {
            ERROR("unknown Ton mode");
            return "ERROR";
        }
    }
}


std::ostream& operator<<(std::ostream& o, const ModeName& mode)
{
    o << tostring(mode);
    return o;
}



} // end namespace pse
