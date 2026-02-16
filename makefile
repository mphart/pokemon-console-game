CFLAGS = -Wall
CLIB = -lm
CFILES = map.c world.c queue.c heap.c main.c
HFILES = map.h world.h queue.h heap.h

all: build

build:
	gcc $(CFLAGS) $(HFILES) $(CFILES) -o main $(CLIB)

clean:
	rm -f ./main

run: build
	./main
