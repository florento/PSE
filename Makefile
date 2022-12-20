CXX = g++
LINKER = g++

CFLAGS = -O3 -Wall -std=c++14 -fPIC
DFLAGS = -DPLATFORM_DARWIN=1 -DPLATFORM_LINUX=2 -DPLATFORM_FREEBSD=3 -DPLATFORM_CYGWIN=4 -DPSE_PLATFORM=PLATFORM_DARWIN

INCLUDES  = -Igeneral -Ipitch -Ipitch/import
INCLUDES += -Iextern/spdlog/include

LIBDIR = lib

# Python related vars
PY_SUFFIX    = $(shell python3-config --extension-suffix)
PY_CFLAGS    = $(shell python3-config --cflags)
PY_LDFLAGS   = $(shell python3-config --ldflags)
PY_INCLUDES  = $(shell python3-config --includes)
PY_INCLUDES += -Iextern/pybind11/include

# sources
SRC  = pitch/NoteName.cpp
SRC += pitch/Accidental.cpp
SRC += pitch/MidiNum.cpp
SRC += pitch/KeyFifth.cpp
SRC += pitch/Fifths.cpp
SRC += pitch/Enharmonic.cpp
SRC += pitch/Ton.cpp
SRC += pitch/Weber.cpp
SRC += pitch/AccidentState.cpp
SRC += pitch/PSCost.cpp
SRC += pitch/PSConfig0.cpp
SRC += pitch/PSConfig.cpp
SRC += pitch/PSBag.cpp
SRC += pitch/PSVector.cpp
SRC += pitch/PSTable.cpp
SRC += pitch/PSPath.cpp
SRC += pitch/import/PSEnum.cpp
SRC += pitch/import/PSRawEnum.cpp
SRC += Speller.cpp
SRC += $(wildcard general/*.cpp) #trace
#SRC += $(wildcard extern/spdlog/src/*.cpp)

# objects and target
OBJDIR = objects
OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)
LINK_TARGET = pse${PY_SUFFIX}

$(LINK_TARGET): $(OBJ) $(OBJDIR)/pypse.o
	$(LINKER) $(CFLAGS) -shared -undefined dynamic_lookup $(OBJ) $(OBJDIR)/pypse.o -o $(LINK_TARGET)
#	$(LINKER) $(CFLAGS) -shared -L$(LIBDIR) -lspdlog -undefined dynamic_lookup $(OBJ) $(OBJDIR)/pypse.o -o $(LINK_TARGET)
	@echo "Linked "$@" successfully!"

$(OBJDIR)/pypse.o : pypse.cpp
	$(CXX) $(CFLAGS) $(DFLAGS) $(INCLUDES) $(PY_INCLUDES) -c pypse.cpp -o $(OBJDIR)/pypse.o
	@echo "Compiled "$<" into "$@" successfully!\n"

$(OBJ): $(OBJDIR)/%.o : %.cpp
	$(CXX) $(CFLAGS) $(DFLAGS) $(INCLUDES) $(PY_INCLUDES) -c $< -o $@
	@echo "Compiled "$<" in "$@" successfully!\n"

.PHONY: clean
clean:
	-rm $(LINK_TARGET) $(OBJ) $(OBJDIR)/pypse.o
	@echo "Cleanup complete!"

show:
	$(info ${SRC}) \
	$(info ${OBJ}) \
	$(info ${LINK_TARGET})
