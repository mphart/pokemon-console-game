#ifndef WORLD_HEADER
#define WORLD_HEADER

#define WORLD_W 401
#define WORLD_H 401
#define WORLD_START_X 200
#define WORLD_START_Y 200
#define WORLD_LOC(x,y) y*WORLD_START_X+x

#include <time.h>
#include <math.h>

#include "map.h"

typedef struct {
    map_t* maps;
    int x;
    int y;
    int seed;
} world_t;

world_t init_world();
world_t seed_world(int seed);
void destroy_world(world_t);

void display_world(world_t w);
void world_move(world_t* w, int);
void world_goto(world_t* w, int x, int y);

#endif