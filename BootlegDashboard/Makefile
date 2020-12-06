DEBUG 		?= 0
PREFIX		:= /opt/bootleg
CXX		:= g++
CXXFLAGS	:= -std=c++2a -Wall -Wpedantic -Wno-unused-parameter -fPIC
LDFLAGS		:= -ldl -pthread -shared
REQLIBS		:=  libodinframework libwirframework libkitengine
LIBS		:= $(shell pkg-config --libs $(REQLIBS))
DEPFLAGS	:= $(shell pkg-config --cflags $(REQLIBS))
BUILDDIR	:= build
OUT_LIBRARY	:= Bootleg.so
SOURCEDIR	:= src
GENSOURCEDIR:= generated
INCLUDEDIR	:= include

SOURCES 	:= $(shell find $(SOURCEDIR) -name '*.cpp')
GENSOURCES	:= $(shell find $(GENSOURCEDIR) -name '*.generated.cpp')
OBJECTS 	:= $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o))  $(addprefix $(BUILDDIR)/,$(GENSOURCES:%.cpp=%.o)) 


ifeq ($(DEBUG), 1)
	CXXFLAGS += -DKIT_DEBUG -DODIN_DEBUG -DWIR_DEBUG -g -O0
else
	CXXFLAGS += -O2 -g
endif

$(OUT_LIBRARY): $(OBJECTS)
	$(shell mkdir lib)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(LDFLAGS) $(LIBS) $(OBJECTS) -o lib/$(OUT_LIBRARY)

$(BUILDDIR)/%.o: %.cpp
	@echo 'Building ${notdir $@} ...'
	$(shell mkdir -p "${dir $@}")
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -I$(INCLUDEDIR) -c $< -o $@

codegen:
	wircodegen --inputPath=./include --outputPath=./generated --include=./include

install:
	mkdir -p $(PREFIX)/Content
	mkdir -p $(PREFIX)/Modules
	cp lib/$(OUT_LIBRARY) $(PREFIX)/Modules/
	#$(shell find ./ \( -name "*.asset" \) -exec cp {} $(PREFIX)/ \;)
	cp -ur ./Content $(PREFIX)/

clean:
	$(shell rm -rf ./build)
	$(shell rm -rf ./lib)
	$(shell rm -f $(OBJECTS) lib/$(OUT_LIBRARY))
