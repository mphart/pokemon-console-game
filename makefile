CFLAGS = -w
CLIB = -lm
CFILES = heap.c map.c character.c world.c main.c
HFILES = heap.h map.h character.h world.h
CSTD = -std=c18 -std=gnu17

all: build

build:
	gcc $(CSTD) $(CFLAGS) $(HFILES) $(CFILES) -o main $(CLIB)

clean:
	rm -f ./main

run: build
	./main
