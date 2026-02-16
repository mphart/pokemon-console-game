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

#define WORLD_W 401
#define WORLD_H 401

typedef enum{
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

typedef struct {
    map_t** maps;
    int x;
    int y;
    int seed;
} world_t;

world_t* init_world();
world_t* seed_world(int seed);
void world_move(world_t* w, Direction d);
void world_goto(world_t* w, int x, int y);
void destroy_world(world_t* w);
void display_world(world_t* w);

#endif