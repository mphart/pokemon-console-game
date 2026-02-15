CFLAGS = -Wall
CLIB = -lm
CFILES = map.c main.c world.c
HFILES = map.h world.h

all: build

build:
	gcc $(CFLAGS) $(HFILES) $(CFILES) -o main $(CLIB)

clean:
	rm -f ./main

run: build
	./main
