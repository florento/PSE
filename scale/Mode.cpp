//
//  Mode.cpp
//  pse
//
//  Created by Florent on 23/03/2023.
//

#include "Mode.hpp"



namespace pse {

Mode::Mode(const std::vector<int>& ls, const std::vector<int>& ln):
_semitons(ls), // vector copy
_csemitons(),  // initially empty
_names(ln),
_cnames()
{
    assert(ls.size() == ln.size());
    int cs = 0;
    int cn = 0;
    for (size_t i = 0; i < ls.size(); ++i)
    {
        assert(_semitons[i] >= 0);
        cs += _semitons[i];
        _csemitons.push_back(cs);
        assert(_names[i] >= 0);
        cn += _names[i];
        _cnames.push_back(cn);
    }
}


Mode::Mode(const Mode& rhs):
_semitons(rhs._semitons), // vector copy
_csemitons(rhs._csemitons),
_names(rhs._names),
_cnames(rhs._cnames)
{
    assert(rhs._semitons.size() == rhs._names.size());
    assert(rhs._semitons.size() == rhs._csemitons.size());
    assert(rhs._names.size() == rhs._cnames.size());
}


Mode::~Mode()
{
    _semitons.clear();
    _csemitons.clear();
    _names.clear();
    _cnames.clear();
}


size_t Mode::size() const
{
    return _semitons.size()+1;
}


} // end namespace pse
