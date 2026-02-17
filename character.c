#include "character.h"

character_t* character_init(CharacterType t, int map_x, int map_y){
    character_t* newChar = calloc(1, sizeof(character_t));
    newChar->type = t;
    newChar->map_x = map_x;
    newChar->map_y = map_y;
    return newChar;
}

void character_destroy(character_t* c){
    free(c);
}

int terrain_cost(map_t* m, int u, int v, CharacterType type){
    TileType tile = m->terrain[tile_id_to_y(v)][tile_id_to_x(v)].type;
    // printf("%d,%d = %d\n", type, tile, terrainCost[type][tile]);
    // return 1;
    return terrainCost[type][tile];
}

costmap_t* character_getCostMap(character_t* c, map_t* map){
    costmap_t* cmap = calloc(1, sizeof(costmap_t));
    cmap->cost = calloc(MAP_W*MAP_H, sizeof(int));
	cmap->prev = calloc(MAP_W*MAP_H, sizeof(int));
    cmap->map = map;
	heap_t* h = heap_init(MAP_W*MAP_H);

    int s = tile_id(c->map_x, c->map_y);

	for(int i = 0; i < MAP_W*MAP_H; i++){
		cmap->cost[i] = i == s ? 0 : INFINITY;
		cmap->prev[i] = -1;
		heap_insert(h, cmap->cost[i], i);
	}

	while(h->size > 0){
		int u = heap_removeMin(h);

		int up = tile_up(u);
		int down = tile_down(u);
		int right = tile_right(u);
		int left = tile_left(u);
        int upLeft = tile_left(up);
        int upRight = tile_right(up);
        int downLeft = tile_left(down);
        int downRight = tile_right(down);

		int alt = cmap->cost[u] + terrain_cost(map, u, up, c->type);
        if(up >= 0 && alt < cmap->cost[up]){
            cmap->cost[up] = alt;
            cmap->prev[up] = u;
            heap_updatePriority(h,cmap->cost[up],up);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, down, c->type);
        if(down < MAP_H*MAP_W && alt < cmap->cost[down]){
            cmap->cost[down] = alt;
            cmap->prev[down] = u;
            heap_updatePriority(h,cmap->cost[down],down);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, right, c->type);
        if(tile_id_to_x(right) > 0 && alt < cmap->cost[right]){
            cmap->cost[right] = alt;
            cmap->prev[right] = u;
            heap_updatePriority(h,cmap->cost[right],right);
        }
		alt = cmap->cost[u] + terrain_cost(map, u, left, c->type);
		if(tile_id_to_x(left) < MAP_W - 1 && alt < cmap->cost[left]){
			cmap->cost[left] = alt;
			cmap->prev[left] = u;
			heap_updatePriority(h,cmap->cost[left],left);
		}
        alt = cmap->cost[u] + terrain_cost(map, u, upRight, c->type);
        if(upRight >= 0 && tile_id_to_x(upRight) > 0 && alt < cmap->cost[upRight]){
            cmap->cost[upRight] = alt;
            cmap->prev[upRight] = u;
            heap_updatePriority(h,cmap->cost[upRight],upRight);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, downRight, c->type);
        if(downRight < MAP_H*MAP_W && tile_id_to_x(downRight) > 0 && alt < cmap->cost[downRight]){
            cmap->cost[downRight] = alt;
            cmap->prev[downRight] = u;
            heap_updatePriority(h,cmap->cost[downRight],downRight);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, upLeft, c->type);
        if(upLeft >= 0 && tile_id_to_x(upLeft) < MAP_W - 1 && alt < cmap->cost[upLeft]){
            cmap->cost[upLeft] = alt;
            cmap->prev[upLeft] = u;
            heap_updatePriority(h,cmap->cost[upLeft],upLeft);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, downLeft, c->type);
        if(downLeft < MAP_H*MAP_W && tile_id_to_x(downLeft) < MAP_W - 1 && alt < cmap->cost[downLeft]){
            cmap->cost[downLeft] = alt;
            cmap->prev[downLeft] = u;
            heap_updatePriority(h,cmap->cost[downLeft],downLeft);
        }
	}

	heap_destroy(h);
	return cmap;
}

void character_destroyCostMap(costmap_t* c){
    free(c->cost);
    free(c->prev);
    free(c);
}

void character_displayCostMap(costmap_t* c){
    for(int i = 0; i < MAP_H; i++){
        for(int j = 0; j < MAP_W; j++){
            if(c->map->terrain[i][j].character == PLAYER){
                printf("@  ");
            } else {
                printf("%2d ", c->cost[tile_id(j,i)]%100);
            }
        }
        printf("\n");
    }
}

void character_print(character_t* c){
    printf("Character: (%d,%d)\n", c->map_x, c->map_y);
}