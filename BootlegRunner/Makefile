DEBUG 		?= 0
PREFIX		:= /opt/bootleg
CXX			:= g++
CXXFLAGS	:= -std=c++2a -Wall -Wpedantic -Wno-unused-parameter -fPIC 
LDFLAGS		:= -ldl -pthread -lvulkan
REQLIBS		:=  libodinframework libwirframework libkitengine 
LIBS		:= $(shell pkg-config --libs $(REQLIBS))
DEPFLAGS	:= $(shell pkg-config --cflags $(REQLIBS))
BUILDDIR	:= build
OUT_EXECUTABLE	:= Bootleg
SOURCEDIR	:= src
INCLUDEDIR	:= include

SOURCES 	:= $(shell find $(SOURCEDIR) -name '*.cpp')
OBJECTS 	:= $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o))


ifeq ($(DEBUG), 1)
	CXXFLAGS += -DKIT_DEBUG -DODIN_DEBUG -DWIR_DEBUG -g -O0
else
	CXXFLAGS += -O2 -g
endif

$(OUT_EXECUTABLE): $(OBJECTS)
	$(shell mkdir bin)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(LDFLAGS) $(LIBS) $(OBJECTS) -o bin/$(OUT_EXECUTABLE)

$(BUILDDIR)/%.o: %.cpp
	@echo 'Building ${notdir $@} ...'
	$(shell mkdir -p "${dir $@}")
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -I$(INCLUDEDIR) -c $< -o $@

install:
	cp bin/$(OUT_EXECUTABLE) $(PREFIX)/

clean:
	$(shell rm -rf ./build)
	$(shell rm -rf ./lib)
	$(shell rm -f $(OBJECTS) lib/$(OUT_EXECUTABLE))
