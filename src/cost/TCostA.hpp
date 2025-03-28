//
//  TCostA.hpp
//  pse
//
//  Created by Florent Jacquemard on 25/02/2025.
//

#ifndef TCostA_hpp
#define TCostA_hpp

#include <iostream>
#include <assert.h>

#include "TCost.hpp"


namespace pse {


/// abstract template (CRTP) od polymorphic cost,
/// extending TCost with a member counting the accidentals.
template<typename T>
struct TCostA : public TCost<TcostA<T>>
{

public: // construction
    
    /// null cost.
    TCostA():
    _accid(0)
    { }
    
    /// copy constructor.
    TCostA(const TCostA& rhs):
    _accid(rhs._accid)
    { }
        
protected: // data
        
    /// number of printed accidentals.
    size_t _accid; // unsigned int
    
};

} // namespace pse

#endif /* TCostA_hpp */

/// @}
