
EXENAME		:= bootleg

DEBUG 		?= 0
PREFIX		:= /opt/bootleg
CXX			:= g++
CXXFLAGS	:= -std=c++2a -Wall -Wpedantic -Wno-unused-parameter -fPIC
LDFLAGS		:= -ldl -pthread 

ifeq ($(DEBUG), 1)
EXEFILENAME	:= $(EXENAME).debug
CXXFLAGS 	+= -DKIT_DEBUG -DODIN_DEBUG -DWIR_DEBUG -g -O0
BUILDDIR	:= build.debug
GENSOURCEDIR:= generated.debug
REQLIBS		:= libkitengine.debug libodinframework.debug libwirframework.debug
else 
EXEFILENAME	:= $(EXENAME)
CXXFLAGS 	+= -O2 -g
BUILDDIR	:= build
GENSOURCEDIR:= generated
REQLIBS		:= libkitengine libodinframework libwirframework
endif

OUT_EXE := bin/$(EXEFILENAME)

LIBS		:= $(shell pkg-config --libs $(REQLIBS))
DEPFLAGS	:= $(shell pkg-config --cflags $(REQLIBS))

SOURCEDIR	:= src
INCLUDEDIR	:= include

SOURCES 	:= $(shell find $(SOURCEDIR) -name '*.cpp')
OBJECTS 	:= $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o))

$(OUT_EXE): $(OBJECTS)
	$(shell mkdir lib)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $(OUT_EXE)

$(BUILDDIR)/%.o: %.cpp
	@echo 'Building ${notdir $@} ...'
	$(shell mkdir -p "${dir $@}")
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -I$(INCLUDEDIR) -c $< -o $@

install:
	cp $(OUT_EXE) $(PREFIX)/

clean:
	$(shell rm -rf ./$(BUILDDIR))
	$(shell rm -f $(OBJECTS) $(OUT_EXE))
