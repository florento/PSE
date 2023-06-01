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
#include "ModeName.hpp"


namespace pse {

class Mode
{
    //friend class ModeFactory;
    
public:
    
    /// undefined mode.
    Mode();
    
    /// predefined mode of given name.
    Mode(const ModeName& name);
    
    /// copy constructor
    Mode(const Mode& rhs);

    /// destructor.
    virtual ~Mode();

    /// number of degrees in this mode.
    size_t size() const;
    
    /// code of this mode.
    inline ModeName name() const { return _name; }

    /// code of this mode.
    inline ModeName code() const { return name(); }

    /// this mode is undefined.
    bool undef() const;
    
    /// distance in 1/2 tones to the tonic.
    /// @param d degree in this mode. the first degree is 0 (not 1).
    const int semitonDistance(size_t d) const;

    /// distance in number of names to the tonic.
    /// @param d degree in this mode. the first degree is 0 (not 1).
    const int nameDistance(size_t d) const;

protected:

    /// code of this mode
    ModeName _name;
    
    /// number of 1/2 tones between one degree and the next.
    std::vector<const int> _semitons;
      
    /// distance to tonic, in 1/2 tones.
    std::vector<int> _csemitons;

    /// number of names between one degree and the next
    std::vector<const int> _names;
    
    /// distance to tonic, in number of names.
    std::vector<int> _cnames;
    
private:
    
    /// internal constructor.
    /// @param name code of this mode.
    /// @param ls list of numbers of 1/2 tones between one degree and the next.
    /// must not be empty.
    /// @param ln list of numbers of names between one degree and the next.
    /// must have same size as ls.
    Mode(const ModeName& name,
         const std::vector<const int>& ls,
         const std::vector<const int>& ln);
    
    /// @param name code of a mode.
    /// @return list of numbers of 1/2 tones between one degree and the next.
    std::vector<const int> predefined_semitons(const ModeName& name);
    
    /// @param name code of a mode.
    /// @return list of number of names between one degree and the next.
    std::vector<const int> predefined_names(const ModeName& name);

};

//std::ostream& operator<<(std::ostream& o, const Mode& ton);

} // end namespace pse

#endif /* Mode_hpp */

/// @} // end group pse


