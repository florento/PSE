//
//  PSWindow.hpp
//  pse
//
//  Created by Florent Jacquemard on 13/02/2025.
//
/// @addtogroup pitch
/// @{

#ifndef PSWindow_hpp
#define PSWindow_hpp

#include <iostream>
#include <iostream>
#include <assert.h>
#include <memory>

#include "pstrace.hpp"
#include "PSRational.hpp"
#include "Pitch.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "MidiNum.hpp"
#include "PSEnum.hpp"




namespace pse {


/// lightweight representation of a sub-enumerator inside a given enumerator.
/// typically for giving access to the content of one bar.
class PSWindow : public PSEnum
{
    
public: // constants

public: // construction
    
    /// Sub-enumerator of the given enumerator with given starting
    /// and ending notes.
    /// @param e embedding enumerator.
    /// @param i0 index of the first note accessible by this enumerator.
    /// must be inside e.
    /// @param i1 index of the note after the last note accessible by this
    /// enumerator.  must be larger than or equal to i0.
    /// must be inside e.
    /// must not be PSEnum::ID_INF - the window is not open.
    PSWindow(PSEnum& e, size_t i0, size_t i1);

    /// Sub-enumerator of the given enumerator spanning over
    /// the bar of given number.
    /// @param e embedding enumerator.
    /// @param b bar number. must be a bar inside e.
    PSWindow(const PSEnum& e, size_t b);

    /// copy constructor.
    PSWindow(const PSWindow& e);
    
    /// destructor
    virtual ~PSWindow();
    
    /// clone this enumerator into a enumerator of the same type.
    /// @todo only for overriding pure virtual method of PSEnum. useless.
    std::unique_ptr<PSEnum> clone() const override;

    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    /// @todo only for overriding pure virtual method of PSEnum. useless.
    std::unique_ptr<PSEnum> clone(size_t i0, size_t i1) const override;

    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    /// @todo only for overriding pure virtual method of PSEnum. useless.
    std::unique_ptr<PSEnum> clone(size_t i0) const override;

public: // access
    
    // the note at the given index is within the interval of notes accessible
    // to this enumerator.
    // @param i index of note.
    // bool inside(size_t i) const override;

    /// midi key number in 0..128 of the note at the given index.
    /// @param i index of a note. must be inside the interval
    /// of this enumerator.
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    virtual unsigned int midipitch(size_t i) const override;

    /// number of the measure containing the note at the given index.
    /// @param i index of a note. must be inside the interval
    /// of this enumerator.
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    virtual long measure(size_t i) const override;

    /// whether the note at the given index is simultaneous with the next note.
    /// @param i index of a note. must be inside the interval
    /// of this enumerator.
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    virtual bool simultaneous(size_t i) const override;

    /// name for the note at the given index, if it has been set,
    /// otherwise Undef.
    /// @param i index of note in the list of input notes.
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    virtual enum NoteName name(size_t i) const override;
    
    /// accidental for the note at the given index, if it has been set,
    /// otherwise Undef.
    /// @param i index of note in the list of input notes.
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    virtual enum Accid accidental(size_t i) const override;
    
    /// octave number for the note at the given index, if it has been set,
    /// otherwise 10.
    /// @param i index of note in the list of input notes.
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    virtual int octave(size_t i) const override;

    /// duration, in number of bars, of the note at the given index,
    /// if it has been set, otherwise 0.
    /// @param i index of note in the list of input notes.
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    virtual long duration_num(size_t i) const override;
    virtual long duration_den(size_t i) const override;

    /// print flag for the note at the given index, if it has been set,
    /// otherwise true.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the list of input notes.
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    virtual bool printed(size_t i) const override;
        
public: // modification
    
    /// set the bounds of interval of accessible indexes to new values.
    /// @param i0 new index of the first note accessible by this window.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to first.
    virtual void reset(size_t i0, size_t i1) override;
    
    /// rename the note at the given index
    /// @param i index of a note in this window.
    /// @param name note name in 'A'..'G'.
    /// @param accid accidental in [-2, 2] where 1 is a half tone
    /// @param oct octave number in -10..10
    /// @param printed whether the accidental must be printed.
    /// @see pse::Pitch::rename()
    /// @todo call analguous of embedding enum,
    /// for overriding a pure virtual method of PSEnum.
    void rename(size_t i,
                const enum NoteName& name, const enum Accid& accid,
                int oct, bool printed) override;
        
protected: // data
    
    /// embedding enumerator
    /// _first and _stop are indices inside this empbedder.
    PSEnum& _container;
    
};


} // namespace pse

#endif /* PSWindow_hpp */

/// @}
