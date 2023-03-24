//
//  Mode.hpp
//  pse
//
//  Created by Florent on 23/03/2023.
//
/// @addtogroup pse
/// @{

#ifndef Mode_hpp
#define Mode_hpp

#include <iostream>
#include <vector>

#include "trace.hpp"
//#include "NoteName.hpp"
//#include "Accidental.hpp"
//#include "Fifths.hpp"
//#include "KeyFifth.hpp"
#include "ModeName.hpp"


namespace pse {

class Mode
{
public:
    /// main constructor.
    /// @param ls list of numbers of 1/2 tones between one degree and the next. must not be empty.
    /// @param ln list of numbers of names between one degree and the next. must have same size as ls.
    Mode(const std::vector<int>& ls, const std::vector<int>& ln);

    /// copy constructor
    Mode(const Mode& rhs);

    /// destructor.
    virtual ~Mode();

    /// number of degrees
    size_t size() const;
    
protected:
    /// number of 1/2 tones between one degree and the next
    std::vector<int> _semitons;
      
    /// distance to tonic, in 1/2 tones
    std::vector<int> _csemitons;

    /// number of names between one degree and the next
    std::vector<int> _names;
    
    /// distance to tonic, in number of names.
    std::vector<int> _cnames;

};


//std::ostream& operator<<(std::ostream& o, const Ton& ton);

} // end namespace pse

#endif /* Mode_hpp */

/// @} // end group pse


