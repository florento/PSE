//
//  PSChord.hpp
//  pse
//
//  Created by Florent Jacquemard on 05/02/2023.
//
/// @addtogroup pitch
/// @{


#ifndef PSChord_hpp
#define PSChord_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <array>
#include <vector>

#include "pstrace.hpp"
#include "PSRational.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "PSEnum.hpp"



namespace pse {

/// sequence of simultaneous notes called a "chord".
/// It can point notes in different voices or in one or several simultaneous
/// chords.
class PSChord : public PSEnum
{
public:

    /// main constructor.
    /// @param e underlying note enumerator.
    /// @param i0 index of the first note accessible by this enumerator.
    PSChord(const PSEnum& e, size_t i0);

    /// copy constructor.
    PSChord(const PSChord& rhs);

    /// destructor
    virtual ~PSChord();
    
    /// clone this enumerator into a enumerator of the same type.
    /// @warning should not be called for this enumerator.
    virtual std::unique_ptr<PSEnum> clone() const override;

    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    /// @warning should not be called for this enumerator.
    virtual std::unique_ptr<PSEnum> clone(size_t i0, size_t i1) const override;

    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    /// @warning should not be called for this enumerator.
    virtual std::unique_ptr<PSEnum> clone(size_t i0) const override;
    
    /// number of notes in this chord.
    /// @see PSEnum::length()
    //inline size_t size() const { return PSEnum::size(); }
    
    /// category of the chord: 2 for interval, 3 for triad etc.
    inline size_t type() const { return _constitution.size(); }
    
    /// number of occurrences of the given pitch class in this chord.
    /// @param c pitch class number, between 0 and 11.
    /// @return number of occurrences of the pitch class c in this chord.
    size_t occurences(unsigned int c) const;

    /// index of one occurrence of the given pitch class in this chord.
    /// @param c pitch class number, between 0 and 11.
    /// @param i index in 0..occurences(c).
    /// @return index of the ith occurrences of the pitch class c in this chord,
    /// in 0..size(). For the index in enumerator, add first().
    size_t occurence(unsigned int c, size_t i) const;

    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note, inside the interval [first(), last() [
    /// of this enumerator.
    unsigned int midipitch(size_t i) const override;

    /// number of measure the note of given index belongs to.
    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a noteinside the interval [first(), last() [
    /// of this enumerator.
    long measure(size_t i) const override;

    /// whether the note of given index is simultaneous with the next note.
    /// @param i index of a note inside the interval [first(), last() [
    /// of this enumerator.
    /// @warning always true for the notes in this enumerator,
    /// except the last one.
    bool simultaneous(size_t i) const override;
    
    /// name for the note of given index, if it has been set,
    /// otherwise Undef.
    /// @param i index of note in the list of input notes.
    enum NoteName name(size_t i) const override;
    
    /// accidental for the note of given index, if it has been set,
    /// otherwise Undef.
    /// @param i index of note in the list of input notes.
    enum Accid accidental(size_t i) const override;
    
    /// octave number for the note of given index, if it has been set,
    /// otherwise 10.
    /// @param i index of note in the list of input notes.
    int octave(size_t i) const override;

    /// duration, in number of bars, of the note of given index,
    /// if it has been set, otherwise 0.
    /// @param i index of note in the list of input notes.
    long duration_num(size_t i) const override;
    long duration_den(size_t i) const override;

    /// print flag for the note of given index, if it has been set,
    /// otherwise true.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the list of input notes.
    bool printed(size_t i) const override;
    
    /// rename the note of given index.
    /// @param i note index inside the interval [first(), last() [
    /// of this enumerator.
    /// @param name note name in 'A'..'G'.
    /// @param accid accidental in [-2, 2] where 1 is a half tone
    /// @param oct octave number in -10..10
    /// @param altprint whether the accidental must be printed.
    /// @warning should not be called for this enumerator.
    virtual void rename(size_t i,
                        const enum NoteName& name, const enum Accid& accid,
                        int oct, bool altprint) override;
    
private: // data

    /// underlying enumerator
    const PSEnum& _enum;
    
    /// sequence of vectors of index of occurrences, for each pitch class,
    /// in the sequence of simultaneous notes.
    /// the vectors of index are ordered by pitch of the corresponding notes.
    /// every index is an index in the enum used to construct this chord.
    std::array<std::vector<size_t>, 12> _occurences;
    //std::array<std::vector<bool>, 12> _occurences;

    // number of occurrence of each pitch class in thos sequence of simultaneous
    // notes. stored separatly from occurences because std::vector<bool> does
    // not provide count similar to std::bitset
    //unsigned int _nboccurences[12];

    // index, in enumerator, of the bass of chord (lowest pitch)
    // size_t _bass;

    /// ordered list of index of constitutive notes of the chord,
    /// starting from the bass.
    /// every index is
    std::vector<size_t> _constitution;

private:
    
    /// @return the index of the first note in enumerator
    /// not simultaneous (and after) the note of index i0,
    /// or ID_INF if there is none.
    static size_t firstNonSimult(const PSEnum& e, size_t i0);

    /// nunmber of notes in chord.
    /// @param e enunmerator which whihchh thihs chohrd is built.
    /// @param i0 index of first note of cohrd in nenunmerator.
    /// @return the number of notes after i0 and
    /// simultaneous with i0.
    static size_t _length(const PSEnum& e, size_t i0);

    /// insert note index i (in enum) into the list _occurences[c]
    /// at the right position (pitch-wise).
    void insert_occurrence(int c, size_t i);
    
    /// initialize the table of occurrences and bass
    void init();
    
    /// initialize the table of consitutive elements
    void init_constitution(size_t bass);

}; // class PSChord


} // namespace pse

#endif /* PSChord_hpp */

/// @}
