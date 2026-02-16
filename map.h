#ifndef MAP_HEADER
#define MAP_HEADER

#define MAP_W 80
#define MAP_H 21
#define NUM_BIOMES 5
#define NUM_SINGLES 30


#define PATH_X_START 4
#define PATH_X_RANGE (MAP_W - 2*PATH_X_START)
#define PATH_Y_START 3
#define PATH_Y_RANGE (MAP_H - 2*PATH_Y_START)

#define BUILDING_X_START 3
#define BUILDING_X_RANGE (MAP_W - 2*BUILDING_X_START)

#include "heap.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

typedef enum {
    UNASSIGNED = 0,
    BOULDER,
    PRAIRIE,
    CLEARING,
    POND,
    FOREST,
    BORDER,
    PATH,
    GATE,
    POKEMART,
    POKECENTER
} Biome;

typedef enum {
    NONE = 0,
    PLAYER,
    HIKER,
    RIVAL,
    SWIMMER,
    OTHER
} Character;

static int terrainCost[6][11] = {
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,INFINITY,20,10,INFINITY,INFINITY,INFINITY,10,10,10,10},
    {0,15,15,10,INFINITY,15,INFINITY,10,INFINITY,50,50},
    {0,INFINITY,20,10,INFINITY,INFINITY,INFINITY,10,INFINITY,50,50},
    {0,INFINITY,INFINITY,INFINITY,7,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY},
    {0,INFINITY,20,10,INFINITY,INFINITY,INFINITY,10,INFINITY,50,50}
};

typedef struct {
    Biome type;
    Character character;
} tile_t;

typedef struct {
    tile_t terrain[MAP_H][MAP_W];
    int nGate;
    int wGate;
    int eGate;
    int sGate;
    bool generated;
} map_t;

map_t* gen_map(int,int,int,int,int);
int print_map(map_t*);
void destroy_map(map_t*);

#endif