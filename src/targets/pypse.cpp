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
#include "PS13.hpp"
#include "PS14.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(pse, m)
{
    m.doc() = "binder to PitchSpelling cpp library, for evaluation";
    
    // test binding
    py::object what = py::cast("What?");
    m.attr("excuseme") = what;

    py::enum_<enum pse::Algo>(m, "Algo", "Pitch Spelling Algo")
        .value("Algo_PSE",   pse::Algo::PSE,   "Engraving based Pitch Spelling")
        .value("Algo_PS13",  pse::Algo::PS13,  "PS 13")
        .value("Algo_PS14",  pse::Algo::PS14,  "PS 14")
        .value("Algo_Undef", pse::Algo::Undef, "Undef")
        .export_values();

    py::enum_<enum pse::NoteName>(m, "NoteName", "Note Names")
        .value("C", pse::NoteName::C, "note C")
        .value("D", pse::NoteName::D, "note D")
        .value("E", pse::NoteName::E, "note E")
        .value("F", pse::NoteName::F, "note F")
        .value("G", pse::NoteName::G, "note G")
        .value("A", pse::NoteName::A, "note A")
        .value("B", pse::NoteName::B, "note B")
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
        .def("fifths", &pse::Ton::fifths, "get key signature")
        .def("undef", &pse::Ton::undef, "ton is undef");
    
    py::class_<pse::PSE>(m, "PSE")
        .def(py::init<>(), "Spell Checker PSE")
        .def("algo", &pse::PSE::algo, "name of spelling algorithm")
        .def("debug", &pse::PSE::debug, "set debug mode", py::arg("on"))
        .def("size", &pse::PSE::size, "number notes to spell")
        .def("add", &pse::PSE::add_pybindwod, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"))
        .def("addlong", &pse::PSE::add_pybindwd, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"))
        .def("nb_tons", &pse::PSE::nbTons,
             "number of tonalities considered for pitch spelling")
        .def("reset_tons", &pse::PSE::resetTons,
             "clear the array of tonalities for pitch spelling")
    // disambiguate overloaded method
        .def("add_ton",
             static_cast<void (pse::PSE::*)(int, pse::ModeName)>(&pse::PSE::addTon),
             "add a tonality for pitch spelling", py::arg("ks"), py::arg("mode"))
        .def("close_tons", &pse::PSE::closeTons, "close the array of tonalities")
        .def("set_global", &pse::PSE::setGlobal, "force global tonality")
        .def("spell", &pse::PSE::spell, "compute spelling")
        .def("rename", &pse::PSE::rename, "rename input notes")
        .def("rename0", &pse::PSE::rename0, "rename input notes (wrt global candidate first pass)")
        .def("rewrite_passing", &pse::PSE::rewritePassing, "rewrite passing notes")
        .def("global_ton", &pse::PSE::global, "get estimated global tonality")
        .def("iglobal_ton", &pse::PSE::iglobal,
             "get index of estimated global tonality")
        .def("keysig", &pse::PSE::fifths, "get estimated global key signature")
        .def("local_bar", &pse::PSE::local, "estimated local tonality for a bar")
             // py::arg("ton"), py::arg("bar"))
        .def("local_note", &pse::PSE::localNote,
             "estimated local tonality for a note")
        .def("globals", &pse::PSE::globals,
             "get number of candidates (ties) for the estimatation of the global tonality")
        .def("globals0", &pse::PSE::globals0,
             "get number of candidates (ties) for the estimatation of the global tonality  (after first pass)")
        .def("global_ton", &pse::PSE::global,
             "get candidate global tonality ")
        .def("global_ton0", &pse::PSE::global0,
             "get candidate global tonality (after first pass)")
        .def("iglobal_ton", &pse::PSE::iglobal,
             "get index of candidate global tonality ")
        .def("iglobal_ton0", &pse::PSE::iglobal0,
             "get index of candidate global tonality (after first pass)")
        .def("name",  &pse::PSE::name, "estimated name of note",
             py::arg("i"))
        .def("accidental", &pse::PSE::accidental,
             "estimated accidental of note", py::arg("i"))
        .def("octave", &pse::PSE::octave, "estimated octave of note",
             py::arg("i"))
        .def("printed", &pse::PSE::printed, "estimated print flag of note",
             py::arg("i"));
    
    py::class_<pse::PS13>(m, "PS13")
        .def(py::init<>(), "Spell Checker PS13")
        .def("algo", &pse::PS13::algo, "name of spelling algorithm")
        .def("debug", &pse::PS13::debug, "set debug mode", py::arg("on"))
        .def("size", &pse::PS13::size, "number notes to spell")
        .def("set_Kpre", &pse::PS13::setKpre, "set the Kpre parameter of PS13")
        .def("set_Kpost", &pse::PS13::setKpost, "set the Kpost parameter of PS13")
        .def("add", &pse::PS13::add_pybindwod, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"))
        .def("addlong", &pse::PS13::add_pybindwd, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"))
        .def("spell", &pse::PS13::spell, "spell notes")
        .def("rewrite_passing", &pse::PS13::rewritePassing, "rewrite passing notes")
        .def("name",  &pse::PS13::name, "estimated name of note",
             py::arg("i"))
        .def("accidental", &pse::PS13::accidental,
             "estimated accidental of note", py::arg("i"))
        .def("octave", &pse::PS13::octave, "estimated octave of note",
             py::arg("i"))
        .def("printed", &pse::PS13::printed, "estimated print flag of note",
             py::arg("i"))
        .def("global_ton", &pse::PS13::global, "estimated global tonality (undef)");
    
    py::class_<pse::PS14>(m, "PS14")
        .def(py::init<>(), "Spell Checker PS14")
        .def("algo", &pse::PS14::algo, "name of spelling algorithm")
        .def("debug", &pse::PS14::debug, "set debug mode", py::arg("on"))
        .def("size", &pse::PS14::size, "number notes to spell")
        .def("add", &pse::PS14::add_pybindwod, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"))
        .def("addlong", &pse::PSE::add_pybindwd, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"))
        .def("nb_tons", &pse::PS14::nbTons,
             "number of tonalities considered for pitch spelling")
        .def("reset_tons", &pse::PS14::resetTons,
             "clear the array of tonalities for pitch spelling")
        // disambiguate overloaded method
        .def("add_ton",
             static_cast<void (pse::PS14::*)(int, pse::ModeName)>(&pse::PS14::addTon),
             "add a tonality for pitch spelling", py::arg("ks"), py::arg("mode"))
        .def("close_tons", &pse::PS14::closeTons, "close the array of tonalities")
        .def("set_global", &pse::PS14::setGlobal, "force global tonality")
        .def("spell", &pse::PS14::spell, "compute spelling")
        .def("rename", &pse::PS14::rename, "rename input notes")
        .def("rename0", &pse::PS14::rename0, "rename input notes (wrt global candidate first pass)")
        .def("rewrite_passing", &pse::PS14::rewritePassing, "rewrite passing notes")
        .def("globals", &pse::PS14::globals,
             "get number of candidates (ties) for the estimatation of the global tonality")
        .def("globals0", &pse::PS14::globals0,
             "get number of candidates (ties) for the estimatation of the global tonality  (after first pass)")
        .def("global_ton", &pse::PS14::global, "get candidate global tonality ")
        .def("global_ton0", &pse::PS14::global0,
             "get candidate global tonality (after first pass)")
        .def("iglobal_ton", &pse::PS14::iglobal,
             "get index of candidate global tonality ")
        .def("iglobal_ton0", &pse::PS14::iglobal0,
             "get index of candidate global tonality (after first pass)")
//        .def("iglobal_ton", &pse::PS14::iglobal,
//             "get index of estimated global tonality")
        .def("keysig", &pse::PS14::fifths, "get estimated global key signature")
        .def("local_bar", &pse::PS14::local, "estimated local tonality for a bar")
             // py::arg("ton"), py::arg("bar"))
        .def("local_note", &pse::PS14::localNote, "estimated local tonality for a note")
        .def("name",  &pse::PS14::name, "estimated name of note",
             py::arg("i"))
        .def("accidental", &pse::PS14::accidental,
             "estimated accidental of note", py::arg("i"))
        .def("octave", &pse::PS14::octave, "estimated octave of note",
             py::arg("i"))
        .def("printed", &pse::PS14::printed, "estimated print flag of note",
             py::arg("i"));
}
