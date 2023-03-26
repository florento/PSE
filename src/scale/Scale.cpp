//
//  Scale.cpp
//  pse
//
//  Created by Florent on 24/03/2023.
//

#include "Scale.hpp"


namespace pse {

Scale::Scale(const Mode& mode, int pc, const enum NoteName& name):
_mode(mode.name()),
_pcs(),
_names(),
_ks((major(mode)||minor(mode))?pc:0, !minor(mode))
{
    assert(0 <= pc);
    assert(pc <= 11);
    _pcs.push_back(pc); // first degree
    _names.push_back(name); // first degree
    for (size_t i = 0; i < mode.size()-1; ++i)
    {
        assert(mode.semitonDistance(i) > 0);
        _pcs.push_back((pc + mode.semitonDistance(i))%12);
        _names.push_back(name + mode.nameDistance(i));
    }
}




// static private
bool Scale::major(const Mode& mode)
{
    return (mode.name() == ModeName::Major);
}


// static private
bool Scale::minor(const Mode& mode)
{
    return ((mode.name() == ModeName::Minor) ||
            (mode.name() == ModeName::MinorNat) ||
            (mode.name() == ModeName::MinorMel));
}


std::ostream& operator<<(std::ostream& o, const Scale& s)
{
    /// TBC
    return o;
}



} // end namespace pse
