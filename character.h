#ifndef CHARACTER_HEADER
#define CHARACTER_HEADER

typedef struct character_t character_t;
typedef struct costmap_t {
	int* cost;
	int* prev
} costmap_t;
typedef enum {
    NONE = 0,
    PLAYER,
    HIKER,
    RIVAL,
    PACER,
    WANDERER,
    SENTRY,
    EXPLORER,
    SWIMMER
} CharacterType;


#include "map.h"
#include "heap.h"

#include <math.h>
#include <memory.h>

struct character_t{
    CharacterType type;
    int start_dir;
    int map_x;
    int map_y;
};

static int terrainCost[9][11] = {
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,99999,20,10,99999,99999,99999,10,10,10,10},
    {0,15,15,10,99999,15,99999,10,99999,50,50},
    {0,99999,20,10,99999,99999,99999,10,99999,50,50},
    {0,99999,20,10,99999,99999,99999,10,99999,50,50},
    {0,99999,20,10,99999,99999,99999,10,99999,50,50},
    {0,99999,20,10,99999,99999,99999,10,99999,50,50},
    {0,99999,20,10,99999,99999,99999,10,99999,50,50},
    {0,99999,99999,99999,7,99999,99999,99999,99999,99999,99999}
};

character_t* character_init(CharacterType t, int map_x, int map_y);
void character_destroy(character_t* c);
costmap_t* character_getCostMap(character_t* c, map_t* m, CharacterType t);
void character_destroyCostMap(costmap_t* cmap);
void character_displayCostMap(costmap_t*);
void character_print(character_t*);
void character_move(character_t*, map_t*);

#endif
