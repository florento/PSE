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
#include "Accid.hpp"
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
        .value("ThreeQuartersSharp", pse::Accid::ThreeQuartersSharp,
               "Three Quarters Sharp")
        .value("Sharp", pse::Accid::Sharp, "Sharp")
        .value("QuarterSharp", pse::Accid::QuarterSharp, "QuarterSharp")
        .value("Natural", pse::Accid::Natural, "Natural")
        .value("QuarterFlat", pse::Accid::QuarterFlat, "Quarter Flat")
        .value("Flat", pse::Accid::Flat, "Flat")
        .value("ThreeQuartersFlat", pse::Accid::ThreeQuartersFlat,
               "Three Quarters Flat")
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
        .value("MajorBlues", pse::ModeName::MajorBlues, "Major Blues")
        .value("MinorBlues", pse::ModeName::MinorBlues, "Minor Blues")
        .export_values();
    
    py::enum_<enum pse::CostType>(m, "CostType", "Cost Type")
        .value("CTYPE_UNDEF", pse::CostType::UNDEF, "Undef type")
        .value("CTYPE_ACCID", pse::CostType::ACCID,
               "number of accidentals without discount for lead tons")
        .value("CTYPE_ACCIDa", pse::CostType::ACCIDa,
               "number of accidentals with equality to one near")
        .value("CTYPE_ACCIDtb", pse::CostType::ACCIDtb,
               "number of accidentals and tiw breaking measures")
        .value("CTYPE_ACCIDtba", pse::CostType::ACCIDtba,
               "number of accidentals and tiw breaking measures")
        .value("CTYPE_ACCIDtbs", pse::CostType::ACCIDtbs,
               "number of accidentals and tiw breaking measures")
        .value("CTYPE_ACCIDtbas", pse::CostType::ACCIDtbas,
               "number of accidentals and tiw breaking measures")
        .value("CTYPE_ACCIDlead", pse::CostType::ACCIDlead,
               "number of accidentals with discount for lead tons (obsolete)")
        .value("CTYPE_ADplus", pse::CostType::ADplus,
               "number of accidentals and distance to local ton, sum of values")
        .value("CTYPE_ADplusa", pse::CostType::ADplusa,
               "number of accidentals and distance to local ton, sum of values")
        .value("CTYPE_ADpluss", pse::CostType::ADpluss,
               "number of accidentals and distance to local ton, sum of values")
        .value("CTYPE_ADplusas", pse::CostType::ADplusas,
               "number of accidentals and distance to local ton, sum of values")
        .value("CTYPE_ADlex", pse::CostType::ADlex,
               "number of accidentals and distance to local ton, lexicographic comparison values")
        .value("CTYPE_ADlexa", pse::CostType::ADlexa,
               "number of accidentals and distance to local ton, lexicographic comparison values")
        .value("CTYPE_ADlexs", pse::CostType::ADlexs,
               "number of accidentals and distance to local ton, lexicographic comparison values")
        .value("CTYPE_ADlexas", pse::CostType::ADlexas,
               "number of accidentals and distance to local ton, lexicographic comparison values")
        .export_values();
    
    py::enum_<enum pse::GridAlgo>(m, "GridAlgo", "Algo for Grid Construction")
        .value("Grid_Best",  pse::GridAlgo::Best, "Greedy algo with best costs")
        .value("Grid_Rank",  pse::GridAlgo::Rank, "Greedy algo with ranks")
        .value("Grid_Exhaustive",  pse::GridAlgo::Exhaustive, "best path search")
        .value("Grid_Undef", pse::GridAlgo::Undef, "Undef")
        .export_values();
    
    py::class_<pse::Ton>(m, "Ton")
    //.def(py::init<>(), "Ton", py::arg("ks"))
        .def("mode", &pse::Ton::getMode, "get mode of ton")
        .def("name", &pse::Ton::getName, "get name of ton")
        .def("accidental", &pse::Ton::getAccidental, "get accidental of ton")
        .def("fifths", &pse::Ton::fifths, "get key signature")
        .def("undef", &pse::Ton::undef, "ton is undef");
  
    // main spelling interface
    py::class_<pse::SpellerEnum>(m, "Speller")
        .def(py::init<size_t, bool>(),
             "Modular Spell Checker, initialized with nb of tons",
             py::arg("tons"), py::arg("aux_enum")=false)
             //py::arg("algo")=pse::Algo::Undef, py::arg("dflag")=false)
        .def("debug", &pse::SpellerEnum::debug,
             "set debug mode", py::arg("on"))
        .def("size", &pse::SpellerEnum::size,
             "number of notes to spell",
             py::arg("aux")=false) // not the auxiliary enumerator
        // .def("reset", &pse::SpellerEnum::reset,
        //     "reset this speller to its initial state, except the list of notes to spell")
        .def("reset_tons", &pse::SpellerEnum::resetTons,
             "clear the array of tonalities for pitch spelling")
        .def("reset_globals", &pse::SpellerEnum::resetGlobals,
             "restore the initial list of global tons (to its state before selection of globals)")
        .def("reset_table", &pse::SpellerEnum::resetTable,
             "clear the current spelling table")
        .def("reset_grid", &pse::SpellerEnum::resetGrid,
             "clear the current grid")
        .def("reset_enum", &pse::SpellerEnum::resetEnum,
             "clear the list of notes to spell")
        .def("has_auxenum", &pse::SpellerEnum::hasAuxEnumerator,
             "the speller has an auxilliary enumerator")
        .def("add_auxenum", &pse::SpellerEnum::addAuxEnumerator,
             "add a new auxilliary enumerator")
        .def("add", &pse::SpellerEnum::add0,
             "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("aux"))
        .def("add_dur", &pse::SpellerEnum::add2,
             "add a new note to spell with duration",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"), py::arg("aux"))
        .def("add_name", &pse::SpellerEnum::add4,
             "add a new note to speller with forced note name",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("name"), py::arg("accid"), py::arg("octave"),
             py::arg("printed"), py::arg("aux"))
        .def("add_namedur", &pse::SpellerEnum::add6,
             "add a new note to speller with forced note name and duration",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"),
             py::arg("name"), py::arg("accid"), py::arg("octave"),
             py::arg("printed"), py::arg("aux"))
        .def("nb_tons", &pse::SpellerEnum::nbTons,
             "number of tonalities considered for pitch spelling")
    // disambiguate overloaded method
        .def("add_ton",
             static_cast<void (pse::Speller::*)(int, pse::ModeName, bool)>(&pse::Speller::addTon3),
             "add a tonality for pitch spelling",
         py::arg("ks"),
         py::arg("mode"),
         py::arg("f_global"))
//        .def("Weber_tonal", &pse::SpellerEnum::WeberTonal,
//             "switch the array of tonalities to tonal mode for Weber dist.")
//        .def("Weber_modal", &pse::SpellerEnum::WeberModal,
//        .def("Weber_modal", &pse::SpellerEnum::WeberModal,
//             "switch the array of tonalities to modal mode for Weber dist.")
        .def("close_tons", &pse::SpellerEnum::closeTons,
             "close the array of tonalities")
        .def("eval_table", &pse::SpellerEnum::evalTable,
             "construct the spelling table")
        .def("reval_table", &pse::SpellerEnum::revalTable,
             "reconstruct the spelling table")
        .def("eval_grid", &pse::SpellerEnum::evalGrid,
             "construct the grid of local tons")
        .def("select_globals", &pse::SpellerEnum::selectGlobals,
             "compute the subarray of tons selected as candidate global tonality, using the spell table",
             py::arg("d") = 0, py::arg("refine") = false)
        .def("select_global", &pse::SpellerEnum::selectGlobal,
             "select a unique ton amongst candidate globals")
        .def("force_global", &pse::SpellerEnum::forceGlobal,
             "reduce the list of global tons to the single given ton")
        .def("rename", &pse::SpellerEnum::rename,
             "rename input notes")
        .def("rewrite_passing", &pse::SpellerEnum::rewritePassing,
             "rewrite passing notes")
        .def("measures",  &pse::SpellerEnum::measures,
             "nb of measures processed by this speller")
        .def("name",  &pse::SpellerEnum::name,
             "estimated name of note",
             py::arg("i"), py::arg("aux")=false)
        .def("accidental", &pse::SpellerEnum::accidental,
             "estimated accidental of note",
             py::arg("i"), py::arg("aux")=false)
        .def("octave", &pse::SpellerEnum::octave,
             "estimated octave of note",
             py::arg("i"), py::arg("aux")=false)
        .def("printed", &pse::SpellerEnum::printed,
             "estimated print flag of note",
             py::arg("i"), py::arg("aux")=false)
        .def("globals", &pse::SpellerEnum::globals,
             "get number of candidates (ties) for the estimatation of the global tonality")
        .def("global_ton", &pse::SpellerEnum::global,
             "get candidate global tonality")
        .def("iglobal_ton", &pse::SpellerEnum::iglobal,
             "get index of candidate global tonality ")
        .def("locals", &pse::PSE::locals,
             "the local tonality grid has been estimated")
        .def("local_bar", &pse::SpellerEnum::local,
             "estimated local tonality for a bar")
             // py::arg("ton"), py::arg("bar"))
        .def("local_note", &pse::SpellerEnum::localNote,
             "estimated local tonality for a note")
        .def("print_grid", &pse::SpellerEnum::printGrid_pybind,
             "print the current grid to standard output")
        .def("print_table", &pse::SpellerEnum::printTable_pybind,
             "print the current table to standard output")
        .def("write_grid", &pse::SpellerEnum::writeGrid,
             "print the current grid to a file")
        .def("write_table", &pse::SpellerEnum::writeTable,
             "print the current table to a file");
   
    // OBSOLETE. replaced by Speller
    py::class_<pse::PSE>(m, "PSE")
        .def(py::init<>(), "Spell Checker PSE")
        .def("algo", &pse::PSE::algo,
             "name of spelling algorithm")
        .def("debug", &pse::PSE::debug,
             "set debug mode", py::arg("on"))
        .def("size", &pse::PSE::size,
             "number of notes to spell")
        .def("add", &pse::SpellerEnum::add0,
             "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("aux"))
        .def("add_dur", &pse::SpellerEnum::add2,
             "add a new note to spell with duration",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"), py::arg("aux"))
        .def("add_name", &pse::SpellerEnum::add4,
             "add a new note to speller with forced note name",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("name"), py::arg("accid"), py::arg("octave"),
             py::arg("printed"), py::arg("aux"))
        .def("add_namedur", &pse::SpellerEnum::add6,
             "add a new note to speller with forced note name and duration",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"),
             py::arg("name"), py::arg("accid"), py::arg("octave"),
             py::arg("printed"), py::arg("aux"))
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
//        .def("set_tonal", &pse::PSE::WeberTonal,
//             "switch the array of tonalities to tonal mode for Weber dist.")
//        .def("set_modal", &pse::PSE::WeberModal,
//             "switch the array of tonalities to modal mode for Weber dist.")
        .def("close_tons", &pse::PSE::closeTons,
             "close the array of tonalities")
        .def("set_global", &pse::PSE::setGlobal,
             "force global tonality")
        .def("spell", &pse::PSE::spell,
             "compute spelling")
        .def("rename", &pse::PSE::rename,
             "rename input notes")
        .def("rename0", &pse::PSE::rename0,
             "rename input notes (wrt global candidate first pass)")
        .def("rewrite_passing", &pse::PSE::rewritePassing,
             "rewrite passing notes")
        .def("global_ton", &pse::PSE::global,
             "get estimated global tonality")
        .def("iglobal_ton", &pse::PSE::iglobal,
             "get index of estimated global tonality")
        .def("keysig", &pse::PSE::fifths,
             "get estimated global key signature")
        .def("locals", &pse::PSE::locals,
             "the local tonality grid has been estimated")
        .def("local_bar", &pse::PSE::local,
             "estimated local tonality for a bar")
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
        .def("name",  &pse::PSE::name,
             "estimated name of note",
             py::arg("i"), py::arg("aux")=false)
        .def("accidental", &pse::PSE::accidental,
             "estimated accidental of note",
             py::arg("i"), py::arg("aux")=false)
        .def("octave", &pse::PSE::octave,
             "estimated octave of note",
             py::arg("i"), py::arg("aux")=false)
        .def("printed", &pse::PSE::printed,
             "estimated print flag of note",
             py::arg("i"), py::arg("aux")=false);
    
    py::class_<pse::PS13>(m, "PS13")
        .def(py::init<>(), "Spell Checker PS13")
        .def("algo", &pse::PS13::algo,
             "name of spelling algorithm")
        .def("debug", &pse::PS13::debug,
             "set debug mode", py::arg("on"))
        .def("size", &pse::PS13::size,
             "number notes to spell")
        .def("reset", &pse::PS13::resetEnum,
             "clear the list of notes to spell")
        .def("set_Kpre", &pse::PS13::setKpre,
             "set the Kpre parameter of PS13")
        .def("set_Kpost", &pse::PS13::setKpost,
             "set the Kpost parameter of PS13")
        .def("add", &pse::SpellerEnum::add0,
             "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("aux"))
        .def("add_dur", &pse::SpellerEnum::add2,
             "add a new note to spell with duration",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"), py::arg("aux"))
        .def("add_name", &pse::SpellerEnum::add4,
             "add a new note to speller with forced note name",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("name"), py::arg("accid"), py::arg("octave"),
             py::arg("printed"), py::arg("aux"))
        .def("add_namedur", &pse::SpellerEnum::add6,
             "add a new note to speller with forced note name and duration",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"),
             py::arg("name"), py::arg("accid"), py::arg("octave"),
             py::arg("printed"), py::arg("aux"))
        .def("spell", &pse::PS13::spell,
             "spell notes")
        .def("rewrite_passing", &pse::PS13::rewritePassing,
             "rewrite passing notes")
        .def("name",  &pse::PS13::name,
             "estimated name of note",
             py::arg("i"), py::arg("aux")=false)
        .def("accidental", &pse::PS13::accidental,
             "estimated accidental of note",
             py::arg("i"), py::arg("aux")=false)
        .def("octave", &pse::PS13::octave,
             "estimated octave of note",
             py::arg("i"), py::arg("aux")=false)
        .def("printed", &pse::PS13::printed,
             "estimated print flag of note",
             py::arg("i"), py::arg("aux")=false)
        .def("locals", &pse::PS13::locals,
             "the local tonality grid has been estimated")
        .def("globals", &pse::PS13::globals,
             "the glocal tonality candidates have been estimated")
        .def("global_ton", &pse::PS13::global,
             "estimated global tonality (undef)");
    
    // OBSOLETE. replaced by Speller
    py::class_<pse::PS14>(m, "PS14")
        .def(py::init<>(), "Spell Checker PS14")
        .def("algo", &pse::PS14::algo,
             "name of spelling algorithm")
        .def("debug", &pse::PS14::debug,
             "set debug mode", py::arg("on"))
        .def("size", &pse::PS14::size,
             "number notes to spell")
        .def("add", &pse::SpellerEnum::add0,
             "add a new note to spell",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("aux"))
        .def("add_dur", &pse::SpellerEnum::add2,
             "add a new note to spell with duration",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"), py::arg("aux"))
        .def("add_name", &pse::SpellerEnum::add4,
             "add a new note to speller with forced note name",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("name"), py::arg("accid"), py::arg("octave"),
             py::arg("printed"), py::arg("aux"))
        .def("add_namedur", &pse::SpellerEnum::add6,
             "add a new note to speller with forced note name and duration",
             py::arg("midi"), py::arg("bar"), py::arg("simultaneous"),
             py::arg("dur_num"), py::arg("dur_den"),
             py::arg("name"), py::arg("accid"), py::arg("octave"),
             py::arg("printed"), py::arg("aux"))
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
//        .def("set_tonal", &pse::PS14::WeberTonal,
//             "switch the array of tonalities to tonal mode for Weber dist.")
//        .def("set_modal", &pse::PS14::WeberModal,
//             "switch the array of tonalities to modal mode for Weber dist.")
        .def("close_tons", &pse::PS14::closeTons,
             "close the array of tonalities")
        .def("set_global", &pse::PS14::setGlobal,
             "force global tonality")
        .def("spell", &pse::PS14::spell,
             "compute spelling")
        .def("rename", &pse::PS14::rename,
             "rename input notes")
        .def("rename0", &pse::PS14::rename0,
             "rename input notes (wrt global candidate first pass)")
        .def("rewrite_passing", &pse::PS14::rewritePassing,
             "rewrite passing notes")
        .def("globals", &pse::PS14::globals,
             "get number of candidates (ties) for the estimatation of the global tonality")
        .def("globals0", &pse::PS14::globals0,
             "get number of candidates (ties) for the estimatation of the global tonality  (after first pass)")
        .def("global_ton", &pse::PS14::global,
             "get candidate global tonality ")
        .def("global_ton0", &pse::PS14::global0,
             "get candidate global tonality (after first pass)")
        .def("iglobal_ton", &pse::PS14::iglobal,
             "get index of candidate global tonality ")
        .def("iglobal_ton0", &pse::PS14::iglobal0,
             "get index of candidate global tonality (after first pass)")
//        .def("iglobal_ton", &pse::PS14::iglobal,
//             "get index of estimated global tonality")
        .def("keysig", &pse::PS14::fifths,
             "get estimated global key signature")
        .def("locals", &pse::PS14::locals,
             "the local tonality grid has been estimated")
        .def("local_bar", &pse::PS14::local,
             "estimated local tonality for a bar")
             // py::arg("ton"), py::arg("bar"))
        .def("local_note", &pse::PS14::localNote,
             "estimated local tonality for a note")
        .def("name",  &pse::PS14::name,
             "estimated name of note",
             py::arg("i"), py::arg("aux")=false)
        .def("accidental", &pse::PS14::accidental,
             "estimated accidental of note",
             py::arg("i"), py::arg("aux")=false)
        .def("octave", &pse::PS14::octave,
             "estimated octave of note",
             py::arg("i"), py::arg("aux")=false)
        .def("printed", &pse::PS14::printed,
             "estimated print flag of note",
             py::arg("i"), py::arg("aux")=false);
}
