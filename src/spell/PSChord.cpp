//
//  PSChord.cpp
//  pse
//
//  Created by Florent Jacquemard on 05/02/2023.
//

#include "PSChord.hpp"



namespace pse {

// main constructor
PSChord::PSChord(const PSEnum& e, size_t i0):
PSEnum(i0, firstNonSimult(e, i0)),
_enum(e)
{
    assert(i0 <= this->stop());
    assert(this->stop() <= e.stop());
    init();
}


// copy constructor
PSChord::PSChord(const PSChord& rhs):
PSEnum(rhs.first(), rhs.stop()),
_occurences(rhs._occurences),  // array, vector copy
_enum(rhs._enum)
{ }


PSChord::~PSChord()
{ }


void PSChord::init()
{
    // trverse all notes in this sequence
    for (size_t i = first(); i < stop(); ++i)
    {
        unsigned int pm = _enum.midipitch(i);
        assert(0 <= pm);
        assert(pm <= 127);
        int c = pm % 12; // chroma (pitch class) of note, in 0..11
        _occurences[c].push_back(i-first());
    }
}


// should not be called
std::unique_ptr<PSEnum> PSChord::clone() const
{
    ERROR("PSChord clone");
    return std::make_unique<PSChord>(*this);
}


// should not be called
std::unique_ptr<PSEnum> PSChord::clone(size_t i0) const
{
    ERROR("PSChord clone {}", i0);
    return std::make_unique<PSChord>(*this, i0);
}


// should not be called
std::unique_ptr<PSEnum> PSChord::clone(size_t i0, size_t i1) const
{
    ERROR("PSChord clone {} {}", i0, i1);
    return std::make_unique<PSChord>(*this, i0); // i1 ignored
}


size_t PSChord::occurences(unsigned int c) const
{
    assert(c < 12);
    return _occurences[c].size();
}


size_t PSChord::occurence(unsigned int c, size_t i) const
{
    assert(c < 12);
    assert(i < occurences(c));
    return _occurences[c][i];
}


// static
size_t PSChord::firstNonSimult(const PSEnum& e, size_t i0)
{
    assert(e.inside(i0));
    for (size_t i1 = i0; ; ++i1)
    {
        // end of enumerator reached
        // should not happen because simult flag = false for last note
        if (i1 == e.stop())
        {
            ERROR("PSChord: end of enum");
            return e.stop();
        }

        if (e.simultaneous(i1))
            continue;
        else
            // index of next note or index of the note after the last note of enum
            return i1 + 1;
    }
}


unsigned int PSChord::midipitch(size_t i) const
{
    return _enum.midipitch(i);
}


long PSChord::measure(size_t i) const
{
    return _enum.measure(i);
}


bool PSChord::simultaneous(size_t i) const
{
    return _enum.simultaneous(i);
}


// should not be called. do nothing.
void PSChord::rename(size_t i, const NoteName& name, const Accid& accid,
                     int oct, bool altprint)
{
    ERROR("PSChord rename {}", i);
}


} // namespace scoremodel

