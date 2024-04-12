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
        .value("Algo_PSE",   pse::Algo::PSE, "Exhaustive Pitch Spelling")
        .value("Algo_PSD",  pse::Algo::PSD, "Deterministic Pitch Spelling")
        .value("Algo_PS13",  pse::Algo::PS13, "Meredith;sPS 13")
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
        .value("MinorMel", pse::ModeName::MinorMel, "Minor Melodic")
        .value("Ionian", pse::ModeName::Ionian, "Ionian")
        .value("Dorian", pse::ModeName::Dorian, "Dorian")
        .value("Phrygian", pse::ModeName::Phrygian, "Phrygian")
        .value("Lydian", pse::ModeName::Lydian, "Lydian")
        .value("Mixolydian", pse::ModeName::Mixolydian, "Mixolydian")
        .value("Aeolian", pse::ModeName::Aeolian, "Aeolian")
        .value("Locrian", pse::ModeName::Locrian, "Locrian")
        .export_values();
    
    py::enum_<enum pse::CostType>(m, "CostType", "Cost Type")
        .value("UNDEF", pse::CostType::UNDEF, "Undef type")
        .value("ACCID", pse::CostType::ACCID,
               "number of accidentals without discount for lead tons")
        .value("ACCIDlead", pse::CostType::ACCIDlead, 
               "number of accidentals with discount for lead tons")
        .value("ADplus", pse::CostType::ADplus, 
               "number of accidentals and distance to local ton, sum of values")
        .value("ADlex", pse::CostType::ADlex,
               "number of accidentals and distance to local ton, lexicographic comparison values")
        .export_values();
    
    py::class_<pse::Ton>(m, "Ton")
    //.def(py::init<>(), "Ton", py::arg("ks"))
        .def("mode", &pse::Ton::getMode, "get mode of ton")
        .def("name", &pse::Ton::getName, "get name of ton")
        .def("accidental", &pse::Ton::getAccidental, "get accidental of ton")
        .def("fifths", &pse::Ton::fifths, "get key signature")
        .def("undef", &pse::Ton::undef, "ton is undef");
  
    py::class_<pse::Speller>(m, "Speller")
        .def(py::init<>(), "Modular Spell Checker")
        .def("debug", &pse::Speller::debug, "set debug mode", py::arg("on"))
        .def("size", &pse::Speller::size, "number of notes to spell")
        .def("add", &pse::Speller::add_pybindwod, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"))
        .def("addlong", &pse::Speller::add_pybindwd, "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"))
        .def("nb_tons", &pse::Speller::nbTons,
             "number of tonalities considered for pitch spelling")
        .def("reset_tons", &pse::Speller::resetTons,
             "clear the array of tonalities for pitch spelling")
    // disambiguate overloaded method
        .def("add_ton",
             static_cast<void (pse::Speller::*)(int, pse::ModeName, bool)>(&pse::Speller::addTon3),
             "add a tonality for pitch spelling",
         py::arg("ks"),
         py::arg("mode"),
         py::arg("f_global"))
        .def("Weber_tonal", &pse::Speller::WeberTonal,
             "switch the array of tonalities to tonal mode for Weber dist.")
        .def("Weber_modal", &pse::Speller::WeberModal,
             "switch the array of tonalities to modal mode for Weber dist.")
        .def("close_tons", &pse::Speller::closeTons, "close the array of tonalities")
        .def("eval_table", &pse::Speller::evalTable, "construct the spelling table")
        .def("reval_table", &pse::Speller::revalTable, "reconstruct the spelling table")
        .def("eval_grid", &pse::Speller::evalGrid, "construct the grid of local tons")
        .def("eval_global", &pse::Speller::evalGlobal,
             "compute the subarray of tons selected as candidate global tonality,")
        .def("rename", &pse::Speller::rename, "rename input notes")
        .def("rewrite_passing", &pse::Speller::rewritePassing, "rewrite passing notes")
        .def("name",  &pse::Speller::name, "estimated name of note",
             py::arg("i"))
        .def("accidental", &pse::Speller::accidental,
             "estimated accidental of note", py::arg("i"))
        .def("octave", &pse::Speller::octave, "estimated octave of note",
             py::arg("i"))
        .def("printed", &pse::Speller::printed, "estimated print flag of note",
             py::arg("i"))
        .def("globals", &pse::Speller::globals,
             "get number of candidates (ties) for the estimatation of the global tonality")
        .def("global_ton", &pse::Speller::global, "get candidate global tonality")
        .def("iglobal_ton", &pse::Speller::iglobal, "get index of candidate global tonality ")
        .def("iglobal_ton", &pse::Speller::iglobal,
             "get index of estimated global tonality")
        .def("locals", &pse::PSE::locals, "the local tonality grid has been estimated")
        .def("local_bar", &pse::Speller::local, "estimated local tonality for a bar")
             // py::arg("ton"), py::arg("bar"))
        .def("local_note", &pse::Speller::localNote, "estimated local tonality for a note");
   
    // TBR ?
    py::class_<pse::PSE>(m, "PSE")
        .def(py::init<>(), "Spell Checker PSE")
        .def("algo", &pse::PSE::algo, "name of spelling algorithm")
        .def("debug", &pse::PSE::debug, "set debug mode", py::arg("on"))
        .def("size", &pse::PSE::size, "number of notes to spell")
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
             static_cast<void (pse::PSE::*)(int, pse::ModeName, bool)>(&pse::PSE::addTon3),
             "add a tonality for pitch spelling",
         py::arg("ks"),
         py::arg("mode"),
         py::arg("f_global"))
        .def("set_tonal", &pse::PSE::WeberTonal,
             "switch the array of tonalities to tonal mode for Weber dist.")
        .def("set_modal", &pse::PSE::WeberModal,
             "switch the array of tonalities to modal mode for Weber dist.")
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
        .def("locals", &pse::PSE::locals, "the local tonality grid has been estimated")
        .def("local_bar", &pse::PSE::local, "estimated local tonality for a bar")
             // py::arg("ton"), py::arg("bar"))
        .def("local_note", &pse::PSE::localNote,
             "estimated local tonality for a note")
        .def("globals", &pse::PSE::globals,
             "get number of candidates (ties) for the estimatation of the global tonality")
        .def("globals0", &pse::PSE::globals0,
             "get number of candidates (ties) for the estimatation of the global tonality (after first pass)")
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
        .def("locals", &pse::PS13::locals, "the local tonality grid has been estimated")
        .def("globals", &pse::PS13::globals, "the glocal tonality candidates have been estimated")
        .def("global_ton", &pse::PS13::global, "estimated global tonality (undef)");
    
    // TBR ?
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
             static_cast<void (pse::PS14::*)(int, pse::ModeName, bool)>(&pse::PSE::addTon3),
             "add a tonality for pitch spelling", 
             py::arg("ks"),
             py::arg("mode"), 
             py::arg("f_global"))
        .def("set_tonal", &pse::PS14::WeberTonal,
             "switch the array of tonalities to tonal mode for Weber dist.")
        .def("set_modal", &pse::PS14::WeberModal,
             "switch the array of tonalities to modal mode for Weber dist.")
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
        .def("locals", &pse::PS14::locals, "the local tonality grid has been estimated")
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
