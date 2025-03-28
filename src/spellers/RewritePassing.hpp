//
//  RewritePassing.hpp
//  pse
//
//  Created by Florent Jacquemard on 08/04/2023.
//
/// @addtogroup pitch
/// @{

#ifndef RewritePassing_hpp
#define RewritePassing_hpp

#include <assert.h>
#include <memory>

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "Speller.hpp"


namespace pse {

/// a special kind of speller that transforms a spelled note enumerator
/// by rewriting passing notes.
class RewritePassing : public Speller
{
public:
    
    /// main constructor.
    /// @param dflag debug mode.
    RewritePassing(bool dflag=false);
    
    /// destructor
    virtual ~RewritePassing();

    /// copy and upgrade.
    RewritePassing(const Speller& rhs);

    /// rewrite the passing notes using the 6 rewrite rules proposed by
    /// D. Meredith in the PS13 Pitch-Spelling algorithm, step 2.
    /// @return whether at least one rewriting was done.
    /// @see PSEnum::rewritePassing()
    virtual bool spell() override;
    
};



} // namespace pse

#endif /* RewritePassing_hpp */

/// @}
