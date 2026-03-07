#ifndef WORLD_HEADER
#define WORLD_HEADER

#define WORLD_W 401
#define WORLD_H 401
#define WORLD_START_X 200
#define WORLD_START_Y 200
#define WORLD_LOC(x,y) y*WORLD_START_X+x

#include "map.h"
#include "character.h"

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


typedef enum{
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

typedef struct {
    map_t** maps;
    int numTrainers;
    int x;
    int y;
    int seed;
    character_t* player;
} world_t;

world_t* init_world(int numTrainers);
world_t* seed_world(int numTrainers, int seed);
void world_move(world_t* w, Direction d);
void world_goto(world_t* w, int x, int y);
void destroy_world(world_t* w);
void display_world(world_t* w);

#endif
