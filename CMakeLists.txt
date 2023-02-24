# Main CMakeList.txt for PSE
# binding for evaluation od Pitch Spelling based on Engraving

cmake_minimum_required(VERSION 3.4...3.18)
#cmake_policy(SET CMP0048 NEW)
#set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")

project(pse LANGUAGES CXX)

# can be 11, 14, 17, 20
set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ version selection") 

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
# # tell to cmake directories where headers file located
include_directories(
  /usr/local/include
  /usr/include
  general
  pitch
  pitch/import
  extern/pybind11/include
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

# ###############################
# spdlog
# ###############################
# build subtree spdlog
#add_subdirectory(extern/spdlog)

# Stand-alone build
#if(NOT TARGET spdlog)
#    # Stand-alone build
#    find_package(spdlog REQUIRED)
#endif()



# ###############################
# Project PSE binder
# ###############################
find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)

# import the pybind11 project (from git subtree in extern)
# provides the command pybind11_add_module
add_subdirectory(extern/pybind11)

# find external installation (on the system)
# find_package(pybind11 CONFIG REQUIRED)

# similar to CMake add_library (wrapper for pybind11)
pybind11_add_module(pse
  src/general/trace.cpp
  src/pitch/NoteName.cpp
  src/pitch/Accidental.cpp
  src/pitch/MidiNum.cpp
  src/pitch/Enharmonic.cpp
  src/pitch/PWO.cpp
  src/pitch/pitch.cpp
  src/interval/IntervalSimple.cpp
  src/interval/Interval.cpp
  src/ton/Fifths.cpp
  src/ton/KeyFifth.cpp
  src/ton/Ton.cpp
  src/ton/TonIndex.cpp
  src/ton/Weber.cpp
  src/spell/PSCost.cpp
  src/spell/PSState.cpp
  src/spell/PSConfig0.cpp
  src/spell/PSConfig.cpp
  src/spell/PSConfig1.cpp
  src/spell/PSConfig2.cpp
  src/spell/PSBag.cpp
  src/spell/PSVector.cpp
  src/spell/PSTable.cpp
  src/spell/PSPath.cpp
  src/import/PSEnum.cpp
  src/import/PSChord.cpp
  src/import/PSRawEnum.cpp
  src/import/PSM21Enum.cpp
  src/targets/Speller.cpp
  src/targets/pypse.cpp
)

# -fvisibility=hidden
# see https://pybind11.readthedocs.io/en/stable/faq.html
set_target_properties(pse PROPERTIES CXX_VISIBILITY_PRESET "hidden")

target_include_directories(pse PUBLIC
  extern/spdlog/include
  src
  src/import
  src/general
  src/pitch
  src/ton
  src/spell
)

# use spdlog pre-compiled library
#target_link_libraries(pse PRIVATE spdlog::spdlog)
