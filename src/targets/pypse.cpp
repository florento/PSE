//
//  pypse.cpp
//  pypse
//
//  Created by Florent Jacquemard on 17/11/2022.
//


#include <iostream>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Ton.hpp"
#include "Speller.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(pse, m)
{
    m.doc() = "binder to PitchSpelling cpp library, for evaluation";

    // test binding
    py::object what = py::cast("What?");
    m.attr("excuseme") = what;
    
    py::enum_<pse::NoteName>(m, "NoteName", "Note Names")
        .value("C",     pse::NoteName::C,    "note C")
        .value("D",     pse::NoteName::D,    "note D")
        .value("E",     pse::NoteName::E,    "note E")
        .value("F",     pse::NoteName::F,    "note F")
        .value("G",     pse::NoteName::G,    "note G")
        .value("A",     pse::NoteName::A,    "note A")
        .value("B",     pse::NoteName::B,    "note B")
        .value("NN_Undef", pse::NoteName::Undef, "Unkown")
        .export_values();

    py::enum_<pse::Accid>(m, "Accid", "Note Names")
        .value("TripleSharp", pse::Accid::TripleSharp, "Triple Sharp")
        .value("DoubleSharp", pse::Accid::DoubleSharp, "Double Sharp")
        .value("ThreeQuartersSharp", pse::Accid::ThreeQuartersSharp, "Three Quarters Sharp")
        .value("Sharp", pse::Accid::Sharp, "Sharp")
        .value("QuarterSharp", pse::Accid::QuarterSharp, "QuarterSharp")
        .value("Natural", pse::Accid::Natural, "Natural")
        .value("QuarterFlat", pse::Accid::QuarterFlat, "Quarter Flat")
        .value("Flat", pse::Accid::Flat, "Flat")
        .value("ThreeQuartersFlat", pse::Accid::ThreeQuartersFlat, "Three Quarters Flat")
        .value("DoubleFlat", pse::Accid::DoubleFlat, "Double Flat")
        .value("TripleFlat", pse::Accid::TripleFlat, "Triple Flat")
        .value("Accid_Undef", pse::Accid::Undef, "Unkown")
        .export_values();

    py::enum_<pse::Ton::Mode>(m, "Mode", "Modes")
        .value("Major", pse::Ton::Mode::Maj, "Major")
        .value("Minor", pse::Ton::Mode::Min, "Minor Harmonic")
        .value("MinorNat", pse::Ton::Mode::MinNat, "Minor Natural")
        .value("MinMel", pse::Ton::Mode::Maj, "Minor Melodic")
        .value("Ionian", pse::Ton::Mode::Ionian, "Ionian")
        .value("Dorian", pse::Ton::Mode::Dorian, "Dorian")
        .value("Phrygian", pse::Ton::Mode::Phrygian, "Phrygian")
        .value("Lydian", pse::Ton::Mode::Lydian, "Lydian")
        .value("Mixolydian", pse::Ton::Mode::Mixolydian, "Mixolydian")
        .value("Eolian", pse::Ton::Mode::Eolian, "Eolian")
        .value("Locrian", pse::Ton::Mode::Locrian, "Locrian")
        .export_values();
    
    py::class_<pse::Speller>(m, "Speller")
        .def(py::init<>(), "Spell Checker")
        .def("debug", &pse::Speller::debug, "set debug mode", py::arg("on"))
        .def("size", &pse::Speller::size, "number notes to spell")
        .def("add", &pse::Speller::add, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"))
        .def("nb_tons", &pse::Speller::nbTons,
             "number of tonalities considered for pitch spelling")
        .def("reset_tons", &pse::Speller::resetTons,
             "clear the array of tonalities for pitch spelling")
        //.def("addton", &pse::Speller::addTon,
        //     "add a tonality for pitch spelling",
        //     py::arg("ks"), py::arg("mode"))
        // disambiguate overloaded method
        .def("add_ton",
             static_cast<void (pse::Speller::*)(int, pse::Ton::Mode)>(&pse::Speller::addTon),
             "add a tonality for pitch spelling", py::arg("ks"), py::arg("mode"))
        .def("set_global", &pse::Speller::setGlobal, "force global tonality")
        .def("spell", &pse::Speller::spell, "spell notes")
        .def("sig", &pse::Speller::fifths, "estimated global tonality")
        .def("name",  &pse::Speller::name, "estimated name of note",
             py::arg("i"))
        .def("accidental", &pse::Speller::accidental,
             "estimated accidental of note", py::arg("i"))
        .def("octave", &pse::Speller::octave, "estimated octave of note",
             py::arg("i"))
        .def("printed", &pse::Speller::printed, "estimated print flag of note",
             py::arg("i"));
}
