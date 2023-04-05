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
#include "PSE.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(pse, m)
{
    m.doc() = "binder to PitchSpelling cpp library, for evaluation";

    // test binding
    py::object what = py::cast("What?");
    m.attr("excuseme") = what;
    
    py::enum_<enum pse::NoteName>(m, "NoteName", "Note Names")
        .value("C",     pse::NoteName::C,    "note C")
        .value("D",     pse::NoteName::D,    "note D")
        .value("E",     pse::NoteName::E,    "note E")
        .value("F",     pse::NoteName::F,    "note F")
        .value("G",     pse::NoteName::G,    "note G")
        .value("A",     pse::NoteName::A,    "note A")
        .value("B",     pse::NoteName::B,    "note B")
        .value("NN_Undef", pse::NoteName::Undef, "Unkown")
        .export_values();

    py::enum_<enum pse::Accid>(m, "Accid", "Note Names")
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

    py::enum_<enum pse::ModeName>(m, "Mode", "Modes")
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
        .def("mode", &pse::Ton::getMode, "get mode of ton")
        .def("name", &pse::Ton::getName, "get name of ton")
        .def("accidental", &pse::Ton::getAccidental, "get accidental of ton")
        .def("fifths", &pse::Ton::fifths, "get key signature");

    py::class_<pse::PSE>(m, "PSE")
        .def(py::init<>(), "Spell Checker")
        .def("debug", &pse::PSE::debug, "set debug mode", py::arg("on"))
        .def("size", &pse::PSE::size, "number notes to spell")
        .def("add", &pse::PSE::add, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"))
        .def("nb_tons", &pse::PSE::nbTons,
             "number of tonalities considered for pitch spelling")
        .def("reset_tons", &pse::PSE::resetTons,
             "clear the array of tonalities for pitch spelling")
        // disambiguate overloaded method
        .def("add_ton",
             static_cast<void (pse::PSE::*)(int, pse::ModeName)>(&pse::PSE::addTon),
             "add a tonality for pitch spelling", py::arg("ks"), py::arg("mode"))
        .def("set_global", &pse::PSE::setGlobal, "force global tonality")
        .def("spell", &pse::PSE::spell, "spell notes")
        .def("global_ton", &pse::PSE::global, "get estimated global tonality")
        .def("iglobal_ton", &pse::PSE::iglobal,
             "get index of estimated global tonality")
        .def("keysig", &pse::PSE::fifths, "get estimated global key signature")
        .def("local_ton", &pse::PSE::local, "get estimated local tonality",
             py::arg("ton"), py::arg("bar"))
        .def("global_cands", &pse::PSE::globalCands,
             "get number of candidates (ties) for the estimatation of the global tonality")
        .def("global_cand_ton", &pse::PSE::globalCand,
             "get candidate global tonality ")
        .def("iglobal_cand_ton", &pse::PSE::iglobalCand,
             "get index of candidate global tonality ")
        .def("name",  &pse::PSE::name, "estimated name of note",
             py::arg("i"))
        .def("accidental", &pse::PSE::accidental,
             "estimated accidental of note", py::arg("i"))
        .def("octave", &pse::PSE::octave, "estimated octave of note",
             py::arg("i"))
        .def("printed", &pse::PSE::printed, "estimated print flag of note",
             py::arg("i"));
}
