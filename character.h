#ifndef CHARACTER_HEADER
#define CHARACTER_HEADER

typedef struct character_t character_t;
typedef struct costmap_t costmap_t;
typedef enum {
    NONE = 0,
    PLAYER,
    HIKER,
    RIVAL,
    SWIMMER,
    OTHER
} CharacterType;


#include "map.h"
#include "heap.h"

#include <math.h>
#include <memory.h>

struct character_t{
    CharacterType type;
    int map_x;
    int map_y;
};

typedef struct costmap_t{
    map_t* map;
    int* cost;
    int* prev;
};

static int terrainCost[6][11] = {
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,99999,20,10,99999,99999,99999,10,10,10,10},
    {0,15,15,10,99999,15,99999,10,99999,50,50},
    {0,99999,20,10,99999,99999,99999,10,99999,50,50},
    {0,99999,99999,99999,7,99999,99999,99999,99999,99999,99999},
    {0,99999,20,10,99999,99999,99999,10,99999,50,50}
};

character_t* character_init(CharacterType t, int map_x, int map_y);
void character_destroy(character_t* c);
costmap_t* character_getCostMap(character_t* c, map_t* m);
void character_destroyCostMap(costmap_t* cmap);

#endif