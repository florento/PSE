//
//  RewritePassing.cpp
//  pse
//
//  Created by Florent Jacquemard on 08/04/2023.
//

#include "RewritePassing.hpp"


namespace pse {


RewritePassing::RewritePassing(bool dflag):
Speller(Algo::RewritePassing, dflag)
{ }


RewritePassing::RewritePassing(const Speller& rhs):
Speller(rhs)
{
    TRACE("update Speller to RewritePassing");
}


/// destructor
RewritePassing::~RewritePassing()
{
    TRACE("delete RewritePassing");
}


bool RewritePassing::spell()
{
    TRACE("Rewriting passing notes");
    return _enum.rewritePassing();
}

} // namespace pse
