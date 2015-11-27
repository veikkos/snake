LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf
FLAGS=-O3
DEBUG_FLAGS=-g
PROFILE_FLAGS=-p

SRC_FILES=$(wildcard *.cpp)
INCLUDES=-I.

BUILD_COMMAND=$(SRC_FILES) $(INCLUDES) $(LIBS)

all:
	g++ $(FLAGS) $(BUILD_COMMAND) -o snake

debug:
	g++ -DDEBUG $(DEBUG_FLAGS) $(BUILD_COMMAND) -o snake-dbg

profile:
	g++ -DPROFILING $(DEBUG_FLAGS) $(PROFILE_FLAGS) $(BUILD_COMMAND) -o snake-prf

clean:
	rm -rf *o snake snake-dbg snake-prf
