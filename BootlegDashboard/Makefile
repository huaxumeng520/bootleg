
LIBNAME		:= dashboard

DEBUG 		?= 0
PREFIX		:= /opt/bootleg
CXX			:= g++
CXXFLAGS	:= -std=c++2a -Wall -Wpedantic -Wno-unused-parameter -fPIC
LDFLAGS		:= -ldl -pthread -shared 

ifeq ($(DEBUG), 1)
LIBFILENAME	:= $(LIBNAME).debug.so
CXXFLAGS 	+= -DKIT_DEBUG -DODIN_DEBUG -DWIR_DEBUG -g -O0
BUILDDIR	:= build.debug
GENSOURCEDIR:= generated.debug
REQLIBS		:= libkitengine.debug libodinframework.debug libwirframework.debug
CODEFLAGS	:= --define=KIT_DEBUG --define=ODIN_DEBUG --define=WIR_DEBUG
else 
LIBFILENAME	:= $(LIBNAME).so
CXXFLAGS 	+= -O2 -g
BUILDDIR	:= build
GENSOURCEDIR:= generated
REQLIBS		:= libkitengine libodinframework libwirframework
CODEFLAGS	:= 
endif

OUT_LIBRARY := lib/$(LIBFILENAME)

LIBS		:= $(shell pkg-config --libs $(REQLIBS))
DEPFLAGS	:= $(shell pkg-config --cflags $(REQLIBS))

SOURCEDIR	:= src
INCLUDEDIR	:= include

SOURCES 	:= $(shell find $(SOURCEDIR) -name '*.cpp')
GENSOURCES	:= $(shell find $(GENSOURCEDIR) -name '*.generated.cpp')
OBJECTS 	:= $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o)) $(addprefix $(BUILDDIR)/,$(GENSOURCES:%.cpp=%.o)) 

$(OUT_LIBRARY): $(OBJECTS)
	$(shell mkdir lib)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $(OUT_LIBRARY)

$(BUILDDIR)/%.o: %.cpp
	@echo 'Building ${notdir $@} ...'
	$(shell mkdir -p "${dir $@}")
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -I$(INCLUDEDIR) -c $< -o $@

install:
	cp $(OUT_LIBRARY) $(PREFIX)/Modules/

install-content:
	cp -ur Content $(PREFIX)/Content

codegen:
	wircodegen --inputPath=$(INCLUDEDIR) --outputPath=$(GENSOURCEDIR) --include=$(INCLUDEDIR) $(CODEFLAGS)

clean:
	$(shell rm -rf ./$(BUILDDIR))
	$(shell rm -rf $(GENSOURCEDIR))
	$(shell rm $(OUT_LIBRARY))
	$(shell rm -f $(OBJECTS) $(OUT_LIBRARY))
