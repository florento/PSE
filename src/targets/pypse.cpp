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
#include "ModeName.hpp"
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

    py::enum_<pse::ModeName>(m, "Mode", "Modes")
        .value("Major", pse::ModeName::Major, "Major")
        .value("Minor", pse::ModeName::Minor, "Minor Harmonic")
        .value("MinorNat", pse::ModeName::MinorNat, "Minor Natural")
        .value("MinMel", pse::ModeName::Major, "Minor Melodic")
        .value("Ionian", pse::ModeName::Ionian, "Ionian")
        .value("Dorian", pse::ModeName::Dorian, "Dorian")
        .value("Phrygian", pse::ModeName::Phrygian, "Phrygian")
        .value("Lydian", pse::ModeName::Lydian, "Lydian")
        .value("Mixolydian", pse::ModeName::Mixolydian, "Mixolydian")
        .value("Aeolian", pse::ModeName::Aeolian, "Aeolian")
        .value("Locrian", pse::ModeName::Locrian, "Locrian")
        .export_values();
    
    py::class_<pse::Ton>(m, "Ton")
        //.def(py::init<>(), "Ton", py::arg("ks"))
        .def("mode", &pse::Ton::mode, "get mode")
        .def("name", &pse::Ton::name, "get note name")
        .def("accidental", &pse::Ton::accidental, "get accidental")
        .def("fifths", &pse::Ton::fifths, "get key signature");

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
        // disambiguate overloaded method
        .def("add_ton",
             static_cast<void (pse::Speller::*)(int, pse::ModeName)>(&pse::Speller::addTon),
             "add a tonality for pitch spelling", py::arg("ks"), py::arg("mode"))
        .def("set_global", &pse::Speller::setGlobal, "force global tonality")
        .def("spell", &pse::Speller::spell, "spell notes")
        .def("global_ton", &pse::Speller::global, "get estimated global tonality")
        .def("iglobal_ton", &pse::Speller::iglobal,
             "get index of estimated global tonality")
        .def("keysig", &pse::Speller::fifths, "get estimated global key signature")
        .def("local_ton", &pse::Speller::local, "get estimated local tonality",
             py::arg("ton"), py::arg("bar"))
        .def("global_cands", &pse::Speller::globalCands,
             "get number of candidates (ties) for the estimatation of the global tonality")
        .def("global_cand_ton", &pse::Speller::globalCand,
             "get candidate global tonality ")
        .def("iglobal_cand_ton", &pse::Speller::iglobalCand,
             "get index of candidate global tonality ")
        .def("name",  &pse::Speller::name, "estimated name of note",
             py::arg("i"))
        .def("accidental", &pse::Speller::accidental,
             "estimated accidental of note", py::arg("i"))
        .def("octave", &pse::Speller::octave, "estimated octave of note",
             py::arg("i"))
        .def("printed", &pse::Speller::printed, "estimated print flag of note",
             py::arg("i"));
}
