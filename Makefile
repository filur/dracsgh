CC=gcc -g
CC_FLAGS=-I/usr/include/SDL
LD_FLAGS=-Wl,--no-as-needed -lm -lSDL -lSDL_ttf

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

all: $(OBJECTS)
	$(CC) $(LD_FLAGS) $(OBJECTS) -o main

%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f main $(OBJECTS)
