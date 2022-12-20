#!/bin/sh
CXX=g++
CFLAGS="-O3 -Wall -std=c++11 -fPIC"
DFLAGS="-DPLATFORM_DARWIN=1 -DPLATFORM_LINUX=2 -DPLATFORM_FREEBSD=3 -DPLATFORM_CYGWIN=4 -DPSE_PLATFORM=PLATFORM_DARWIN"
INCLUDES="-Igeneral -Ipitch -Ipitch/import"
PYTHON_INCLUDES="$(python3-config --includes) -Iextern/pybind11/include"
$CXX $CFLAGS $DFLAGS $INCLUDES $PYTHON_INCLUDES pypse.cpp


$CXX $CFLAGS $DFLAGS $INCLUDES $PYTHON_INCLUDES -undefined dynamic_lookup pypse.cpp
