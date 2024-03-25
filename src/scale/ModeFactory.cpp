//
//  ModeFactory.cpp
//  pse
//
//  Created by Florent on 23/03/2023.
//

#include "ModeFactory.hpp"


namespace pse {


// see ref e.g. in https://muted.io/scale-formulas-intervals/
std::shared_ptr<Mode> ModeFactory::make(ModeName name)
{
    switch (name)
    {
        case ModeName::Major:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 2, 1, 2, 2, 2, 1},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});
        // harmonic minor
        case ModeName::Minor:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 1, 2, 2, 1, 3, 1},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});

        case ModeName::MinorNat:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 1, 2, 2, 1, 2, 2},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});

        case ModeName::MinorMel:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 1, 2, 2, 2, 2, 1},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});
        // Mode of C = Major
        case ModeName::Ionian:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 2, 1, 2, 2, 2, 1},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});
        // Mode of D
        case ModeName::Dorian:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 1, 2, 2, 2, 1, 2},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});

        // Mode of E
        case ModeName::Phrygian:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{1, 2, 2, 2, 1, 2, 2},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});
        // Mode of F
        case ModeName::Lydian:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 2, 2, 1, 2, 2, 1},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});
        // Mode of G
        case ModeName::Mixolydian:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 2, 1, 2, 2, 1, 2},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});

        // Mode of A
        case ModeName::Aeolian:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 1, 2, 2, 1, 2, 2},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});

        // Mode of B
        case ModeName::Locrian:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{1, 2, 2, 1, 2, 2, 2},
                               std::vector<const int>{1, 1, 1, 1, 1, 1, 1});

        // Harmonic Chromatic
        // all notes are repeated except tonic and dominant
        case ModeName::Chromatic:
             return std::make_shared<Mode>(name,
                std::vector<const int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                std::vector<const int>{1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1});
                
        // Major Pentatonic = Major - 4th and 7th degrees
        /// @todo is it the correct spelling?
        /// @see https://muted.io/major-pentatonic-scale
        case ModeName::Major5:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 2, 3, 2, 3},
                               std::vector<const int>{1, 1, 2, 1, 2});

        // Minor Pentatonic =  natural minor - 2nd and 6th degrees.
        case ModeName::Minor5:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{1, 2, 2, 1, 2},
                               std::vector<const int>{2, 1, 1, 2, 1});

        // = Major5 + flat 3d blue note
        /// @todo is it the correct spelling?
        /// @see https://muted.io/major-blues-scale
        case ModeName::MajorBlues:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 1, 1, 3, 2, 3},
                               std::vector<const int>{1, 1, 1, 1, 1, 2});

        // = Minor5 + flat 5th blue note
        /// @todo is it the correct spelling?
        /// @see https://muted.io/minor-blues-scale
        case ModeName::MinorBlues:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{3, 2, 1, 1, 3, 2},
                               std::vector<const int>{2, 1, 1, 1, 0, 2});

        // Augmented Scale = symmetrical augmented scale = two augmented triads
        /// @todo is it the correct spelling?
        /// @see https://muted.io/augmented-scale
        case ModeName::Augmented:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{3, 1, 3, 1, 3, 1},
                               std::vector<const int>{1, 1, 2, 1, 2, 1});


        // Diminished Scale = symmetric diminished scale
        //                  = two diminished 7th chords
        /// @todo is it the correct spelling? not diatonic
        /// @see https://muted.io/diminished-scale
        case ModeName::Diminished:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{2, 1, 2, 1, 2, 1, 2, 1},
                               std::vector<const int>{1, 1, 1, 1, 1, 0, 1, 1});

        // Diminished Scale half-whole
        case ModeName::DiminishedHW:
            return std::make_shared<Mode>(name,
                               std::vector<const int>{1, 2, 1, 2, 1, 2, 1, 2},
                               std::vector<const int>{1, 1, 0, 1, 1, 1, 0, 1});
            
        case ModeName::Undef:
        {
            ERROR("ModeFactory: mode name should not be Undef");
            return nullptr;
        }
            
        default:
        {
            ERROR("ModeFactory: unknown mode name {}", name);
            return nullptr;
        }
    }
}



} // end namespace pse
