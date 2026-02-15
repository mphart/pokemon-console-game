#ifndef MAP_HEADER
#define MAP_HEADER

#define MAP_W 80
#define MAP_H 21

#define TILE_UP(x) x - MAP_W
#define TILE_RIGHT(x) x + 1
#define TILE_LEFT(x) x - 1
#define TILE_DOWN(x) x + MAP_W

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    char* terrain;
    int nGate;
    int wGate;
    int eGate;
    int sGate;
} map_t;

map_t gen_map(int,int,int,int,int);
void print_map(map_t);
void destroy_map(map_t);

#endif
