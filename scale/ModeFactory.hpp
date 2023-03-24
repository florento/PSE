//
//  ModeFactory.hpp
//  pse
//
//  Created by Florent on 23/03/2023.
//
/// @addtogroup pse
/// @{

#ifndef ModeFactory_hpp
#define ModeFactory_hpp

#include <iostream>
#include <memory>

#include "trace.hpp"
#include "ModeName.hpp"
#include "Mode.hpp"


namespace pse {


/// generator of predefined modes.
class ModeFactory
{
    
public:
    
    ~ModeFactory(){};
    
    /// generator of default mode for name.
    /// @param name mode name. most not be Undef.
    /// @return new Mode corresponding to name. NULL if name is Undef.
    std::shared_ptr<Mode> make(ModeName name);

};


} // end namespace pse

#endif /* ModeFactory_hpp */

/// @} // end group pse

