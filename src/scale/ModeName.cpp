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

        case ModeName::Major:
            return "major";

        case ModeName::Minor:
            return "minor";

        case ModeName::MinorNat:
            return "minor nat";

        case ModeName::MinorMel:
            return "minor mel";

        case ModeName::Ionian:
            return "ionian";

        case ModeName::Dorian:
            return "dorian";

        case ModeName::Phrygian:
            return "phrygian";

        case ModeName::Lydian:
            return "lydian";

        case ModeName::Mixolydian:
            return "mixolydian";
            
        case ModeName::Aeolian:
            return "aeolian";

        case ModeName::Locrian:
            return "locrian";

        case ModeName::Major5:
            return "major Pentatonic";

        case ModeName::Minor5:
            return "minor pentatonic";

        case ModeName::MajorBlues:
            return "major blues";

        case ModeName::MinorBlues:
            return "minor blues";

        case ModeName::Augmented:
            return "augmented";

        case ModeName::Diminished:
            return "diminished";

        case ModeName::Chromatic:
            return "harmonic chromatic";

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
