CXX = g++
LINKER = g++

CFLAGS = -O3 -Wall -std=c++14 -fPIC
DFLAGS = -DPLATFORM_DARWIN=1 -DPLATFORM_LINUX=2 -DPLATFORM_FREEBSD=3 -DPLATFORM_CYGWIN=4 -DPSE_PLATFORM=PLATFORM_DARWIN

INCLUDES  = -Isrc
INCLUDES += -Isrc/general -Isrc/pitch -Isrc/ton -Isrc/spell -Isrc/import
INCLUDES += -Iextern/spdlog/include

LIBDIR = lib

# Python related vars
PY_SUFFIX    = $(shell python3-config --extension-suffix)
PY_CFLAGS    = $(shell python3-config --cflags)
PY_LDFLAGS   = $(shell python3-config --ldflags)
PY_INCLUDES  = $(shell python3-config --includes)
PY_INCLUDES += -Iextern/pybind11/include

# sources
SRC = src/general/trace.cpp
SRC += src/pitch/NoteName.cpp
SRC += src/pitch/Accid.cpp
SRC += src/pitch/MidiNum.cpp
SRC += src/pitch/Fifths.cpp
SRC += src/pitch/Enharmonic.cpp
SRC += src/ton/KeyFifth.cpp
SRC += src/ton/Ton.cpp
SRC += src/ton/Weber.cpp
SRC += src/spell/PSState.cpp
SRC += src/spell/PSCost.cpp
SRC += src/spell/PSConfig0.cpp
SRC += src/spell/PSConfig.cpp
SRC += src/spell/PSBag.cpp
SRC += src/spell/PSVector.cpp
SRC += src/spell/PSTable.cpp
SRC += src/spell/PSPath.cpp
SRC += src/import/PSEnum.cpp
SRC += src/import/PSRawEnum.cpp
SRC += src/import/PSM21Enum.cpp
SRC += src/Speller.cpp
SRC += $(wildcard general/*.cpp) #trace
#SRC += $(wildcard extern/spdlog/src/*.cpp)

# objects and target
OBJDIR = build/objects
OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)
LINK_TARGET = pse${PY_SUFFIX}
dir_guard=@mkdir -p $(@D)

$(LINK_TARGET): $(OBJ) $(OBJDIR)/pypse.o
	$(dir_guard)
	$(LINKER) $(CFLAGS) -shared -undefined dynamic_lookup $(OBJ) $(OBJDIR)/pypse.o -o $(LINK_TARGET)
#	$(LINKER) $(CFLAGS) -shared -L$(LIBDIR) -lspdlog -undefined dynamic_lookup $(OBJ) $(OBJDIR)/pypse.o -o $(LINK_TARGET)
	@echo "Linked "$@" successfully!"

$(OBJDIR)/pypse.o : src/pypse.cpp
	$(dir_guard)
	$(CXX) $(CFLAGS) $(DFLAGS) $(INCLUDES) $(PY_INCLUDES) -c src/pypse.cpp -o $(OBJDIR)/pypse.o
	@echo "Compiled "$<" into "$@" successfully!\n"

$(OBJ): $(OBJDIR)/%.o : %.cpp
	$(dir_guard)
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
