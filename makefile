CFLAGS = -Wall
CLIB = -lm
CFILES = terrain.c main.c
HFILES = terrain.h

all: build

build:
	gcc $(CLIB) $(CFLAGS) $(HFILES) $(CFILES) -o main

clean:
	rm -f ./main

run: build
	./main
