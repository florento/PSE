//
//  Mode.cpp
//  pse
//
//  Created by Florent on 23/03/2023.
//

#include "Mode.hpp"



namespace pse {

Mode::Mode(const ModeName& name,
           const std::vector<const int>& ls,
           const std::vector<const int>& ln):
_name(name),
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
        assert(_semitons.at(i) >= 0);
        cs += _semitons.at(i);
        //const int ccs = cs;
        _csemitons.push_back(cs);
        assert(_names.at(i) >= 0);
        cn += _names.at(i);
        //const int ccn = cn;
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
//    _semitons.clear();
//    _csemitons.clear();
//    _names.clear();
//    _cnames.clear();
}


size_t Mode::size() const
{
    return _semitons.size()+1;
}


const int Mode::semitonDistance(size_t i) const
{
    assert(i < _csemitons.size());
    return _csemitons.at(i);
}


const int Mode::nameDistance(size_t i) const
{
    assert(i < _cnames.size());
    return _cnames.at(i);
}



} // end namespace pse
