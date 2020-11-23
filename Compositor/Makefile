DEPS=miral
DEPS_CFLAGS=$(shell pkg-config --cflags $(DEPS))
DEPS_LIBS=$(shell pkg-config --libs $(DEPS))
PROJ_CFLAGS=-std=c++20 -I./include
ALL_CFLAGS=$(CFLAGS) $(DEPS_CFLAGS) $(PROJ_CFLAGS)

BootlegCompositor: obj/main.o obj/Authorizer.o obj/WindowManager.o
	$(CXX) $(ALL_CFLAGS) -o $@ $^ $(DEPS_LIBS)

obj/main.o: src/main.cpp
	$(CXX) $(ALL_CFLAGS) -c $< -o $@
	
obj/WindowManager.o: src/WindowManager.cpp
	$(CXX) $(ALL_CFLAGS) -c $< -o $@
		
obj/Authorizer.o: src/Authorizer.cpp
	$(CXX) $(ALL_CFLAGS) -c $< -o $@
		
clean:
	rm -f BootlegCompositor 
	rm -rf ./obj/*

.DEFAULT_GOAL=BootlegCompositor
.PHONY: clean
