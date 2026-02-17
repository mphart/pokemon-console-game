CFLAGS = -w
CLIB = -lm
CFILES = heap.c map.c character.c world.c main.c
HFILES = heap.h map.h character.h world.h

all: build

build:
	gcc $(CFLAGS) $(HFILES) $(CFILES) -o main $(CLIB)

clean:
	rm -f ./main

run: build
	./main
