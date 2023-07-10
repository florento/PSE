# Main CMakeList.txt for PSE
# binding for evaluation od Pitch Spelling based on Engraving

cmake_minimum_required(VERSION 3.4...3.18)
cmake_policy(SET CMP0048 NEW)
set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")

project(pse LANGUAGES CXX)

# can be 11, 14, 17, 20
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ version selection")

# optional, ensure standard is supported
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# optional, keep compiler extensions off
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument -Wno-nullability-completeness -Wno-deprecated")

# # set LD flags
# set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pthread" )


# ###############################
# include
# ###############################

set(DIRS
  src/general
  src/import
  src/pitch
  src/interval
  src/chord
  src/ton
  src/scale
  src/cost
  src/table
  src/spellers
  src/spellers/PS13
  src/spellers/PS14
  src/spellers/PSE
  src/targets
)


# # tell to cmake directories where headers file located
include_directories(
  /usr/local/include
  /usr/include
  extern/pybind11/include
  extern/spdlog/include
  ${DIRS}
)

################################
# link
################################

# standard library directories
#link_directories(/usr/local/lib /usr/lib extern/googletest/googletest/lib)

################################
# target compile option 
################################
## check OS type
MESSAGE( STATUS "CMAKE_SYSTEM_NAME:         " ${CMAKE_SYSTEM_NAME} )
string(TOUPPER ${CMAKE_SYSTEM_NAME} CMAKE_UPPER_SYSTEM_NAME)

## specify definitions
add_definitions(
  -DPLATFORM_DARWIN=1
  -DPLATFORM_LINUX=2
  -DPLATFORM_FREEBSD=3
  -DPLATFORM_CYGWIN=4
  -DPSE_PLATFORM=PLATFORM_${CMAKE_UPPER_SYSTEM_NAME}
)

################################
# spdlog
################################
# build subtree spdlog
#add_subdirectory(extern/spdlog)

# Stand-alone build
#if(NOT TARGET spdlog)
#    # Stand-alone build
#    find_package(spdlog REQUIRED)
#endif()

################################
# googletest
################################
#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
#
#enable_testing()
#
#add_executable(
#  unit_test
#  TestScale.cpp
#)
#target_link_libraries(
#  hello_test
#  GTest::gtest_main
#)
#
#include(GoogleTest)
#gtest_discover_tests(unit_test)

################################
# find Python3
################################
#find_package (Python3 COMPONENTS Interpreter Development)
find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)

#if (NOT Python3_FOUND)
#  message(FATAL_ERROR "Could not find path to python3")
#endif (NOT Python3_FOUND)

################################
# Project PSE binder
################################

# import the pybind11 project (from git subtree in extern)
# provides the command pybind11_add_module
add_subdirectory(extern/pybind11)

# find external installation (on the system)
# find_package(pybind11 CONFIG REQUIRED)

set(SOURCES
  src/general/trace.cpp
  src/general/Rational.cpp
  src/general/utils.cpp
  src/pitch/NoteName.cpp
  src/pitch/Accidental.cpp
  src/pitch/MidiNum.cpp
  src/pitch/Enharmonic.cpp
  src/pitch/PWO.cpp
  src/pitch/Pitch.cpp
  src/interval/IntervalSimple.cpp
  src/interval/Interval.cpp
  src/chord/PSChord.cpp
  src/ton/Fifths.cpp
  src/ton/KeyFifth.cpp
  src/ton/Ton.cpp
  src/ton/TonIndex.cpp
  src/ton/Weber.cpp
  src/scale/Mode.cpp
  src/scale/ModeFactory.cpp
  src/scale/ModeName.cpp
  src/scale/Scale.cpp
  src/import/PSEnum.cpp
  src/import/PSRawEnum.cpp
  src/cost/Cost.cpp
  src/cost/CostAD.cpp
  src/cost/CostADlex.cpp
  src/cost/CostADplus.cpp
  src/table/PSState.cpp
  src/table/PSOrder.cpp
  src/table/PSConfig0.cpp
  src/table/PSConfig.cpp
  src/table/PSConfig1.cpp
  src/table/PSConfig1c.cpp
  src/table/PSBag.cpp
  src/table/PSVector.cpp
  src/table/PSTable.cpp
  src/table/PSGrid.cpp
  src/table/PSGlobal.cpp
  src/table/PSPath.cpp
  src/spellers/AlgoName.cpp
  src/spellers/Speller.cpp
  src/spellers/Speller1pass.cpp
  src/spellers/Speller2pass.cpp
  src/spellers/RewritePassing.cpp
  src/spellers/PS13/PS13.cpp
  src/spellers/PS14/PS14.cpp
  src/spellers/PSE/PSE.cpp
  src/targets/pypse.cpp
)

# similar to CMake add_library (wrapper for pybind11)
pybind11_add_module(pse
  ${SOURCES}
  src/targets/pypse.cpp
)

# -fvisibility=hidden
# see https://pybind11.readthedocs.io/en/stable/faq.html
set_target_properties(pse PROPERTIES CXX_VISIBILITY_PRESET "hidden")

target_include_directories(pse PUBLIC
  extern/spdlog/include
  extern/pybind11/include
  ${DIRS}
)

# use spdlog pre-compiled library
#target_link_libraries(pse PRIVATE spdlog::spdlog)
