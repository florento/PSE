//
//  exemple.cpp
//  pypse
//
//  Created by Florent Jacquemard on 21/11/2022.
//

#include <iostream>
#include <string>

#include "pybind11/pybind11.h"
// #include "pybind11/stl.h"

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/fmt/ostr.h" // support for user defined types

#define ERROR(...) spdlog::error(__VA_ARGS__)
#define WARN(...)  spdlog::warn(__VA_ARGS__)
#define INFO(...)  spdlog::info(__VA_ARGS__)
#define TRACE(...) spdlog::trace(__VA_ARGS__)
#define DEBUG(...) spdlog::debug(__VA_ARGS__)


namespace py = pybind11;

int add(int i, int j)
{
    return i + j;
}



struct Printer
{
    Printer(const std::string& name): name(name)
    { spdlog::set_pattern("[%^pse_%l%$] %v"); }
    void setName(const std::string& name_) { name = name_; }
    const std::string& getName() const { return name; }
    void hi()
    { INFO("{}: NNNNNnnniii {}", name, 1); }

    std::string name;
};


PYBIND11_MODULE(exemple, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring
    
    m.def("add", &add, "A function that adds two numbers");
    
    // with named args
    m.def("addargs", &add, "A function which adds two numbers",
          py::arg("i"), py::arg("j"));
    // or
    // using namespace pybind11::literals;
    // m.def("add2", &add, "i"_a, "j"_a);
    // where "i"_a is an ugly shorthand for py::arg("i")

    // with named args and default values
    m.def("addd", &add, "A function which adds two numbers",
          py::arg("i") = 1, py::arg("j") = 1);
    // defaults args of C++ are NOT transmitted to the binding

    // constant value as object: use cast
    py::object what = py::cast("Wwhat?");
    m.attr("excuseme") = what;
    
    py::class_<Printer>(m, "Printer")
        .def(py::init<const std::string &>())
        .def("setName", &Printer::setName)
        .def("getName", &Printer::getName)
        .def("hi", &Printer::hi);
}
