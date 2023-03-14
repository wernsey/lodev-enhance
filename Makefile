CC=g++
CPPFLAGS=-c -Wall
LDFLAGS= -lmingw32 -lSDLmain -lSDL

# TODO: Remove this later
BUILD=debug

# Detect operating system:
# More info: http://stackoverflow.com/q/714100
ifeq ($(OS),Windows_NT)
  LDFLAGS += -mwindows
endif

ifeq ($(BUILD),debug)
  # Debug
  CPPFLAGS += -O0 -g
  LDFLAGS +=
else
  # Release mode
  CPPFLAGS += -O2 -DNDEBUG
  LDFLAGS += -s
endif

SOURCES=quickcg.cpp raycaster.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: main

debug:
	make BUILD=debug

main: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CPPFLAGS) $< -o $@

quickcg.o: quickcg.cpp quickcg.h
raycaster.o: raycaster.cpp quickcg.h

.PHONY : clean deps

deps:
	$(CC) -MM $(SOURCES)

clean:
	-rm -f main
	-rm -f *.o