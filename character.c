#include "character.h"

character_t* character_init(CharacterType t, int map_x, int map_y){
    character_t* newChar = calloc(1, sizeof(character_t));
    newChar->type = t;
    newChar->map_x = map_x;
    newChar->map_y = map_y;
    newChar->start_dir = 1;
    return newChar;
}

void character_destroy(character_t* c){
    free(c);
}

int terrain_cost(map_t* m, int u, int v, CharacterType type){
//	return 1;
    if(u < 0 || u >= MAP_H * MAP_W || v < 0 || v >= MAP_H*MAP_W){
	    return 99999;
	   }
    TileType tile = m->terrain[tile_id_to_y(v)][tile_id_to_x(v)].type;
//  printf("%d,%d = %d\n", type, tile, terrainCost[type][tile]);
    return terrainCost[type][tile];
}

costmap_t* character_getCostMap(character_t* c, map_t* map, CharacterType t){
    costmap_t* cmap = calloc(1, sizeof(costmap_t));
    cmap->cost = calloc(MAP_W*MAP_H, sizeof(int));
	cmap->prev = calloc(MAP_W*MAP_H, sizeof(int));
	heap_t* h = heap_init(MAP_W*MAP_H);

    int s = tile_id(c->map_x, c->map_y);

	for(int i = 0; i < MAP_W*MAP_H; i++){
		cmap->cost[i] = i == s ? 0 : 99999;
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

	;
		int alt = cmap->cost[u] + terrain_cost(map, u, up, t);
        if(up >= 0 && alt < cmap->cost[up]){
            cmap->cost[up] = alt;
            cmap->prev[up] = u;
            heap_updatePriority(h,cmap->cost[up],up);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, down, t);
        if(down < MAP_H*MAP_W && alt < cmap->cost[down]){
            cmap->cost[down] = alt;
            cmap->prev[down] = u;
            heap_updatePriority(h,cmap->cost[down],down);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, right, t);
        if(tile_id_to_x(right) > 0 && alt < cmap->cost[right]){
            cmap->cost[right] = alt;
            cmap->prev[right] = u;
            heap_updatePriority(h,cmap->cost[right],right);
        }
		alt = cmap->cost[u] + terrain_cost(map, u, left, t);
		if(tile_id_to_x(left) < MAP_W - 1 && alt < cmap->cost[left]){
			cmap->cost[left] = alt;
			cmap->prev[left] = u;
			heap_updatePriority(h,cmap->cost[left],left);
		}
        alt = cmap->cost[u] + terrain_cost(map, u, upRight, t);
        if(upRight >= 0 && tile_id_to_x(upRight) > 0 && alt < cmap->cost[upRight]){
            cmap->cost[upRight] = alt;
            cmap->prev[upRight] = u;
            heap_updatePriority(h,cmap->cost[upRight],upRight);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, downRight, t);
        if(downRight < MAP_H*MAP_W && tile_id_to_x(downRight) > 0 && alt < cmap->cost[downRight]){
            cmap->cost[downRight] = alt;
            cmap->prev[downRight] = u;
            heap_updatePriority(h,cmap->cost[downRight],downRight);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, upLeft, t);
        if(upLeft >= 0 && tile_id_to_x(upLeft) < MAP_W - 1 && alt < cmap->cost[upLeft]){
            cmap->cost[upLeft] = alt;
            cmap->prev[upLeft] = u;
            heap_updatePriority(h,cmap->cost[upLeft],upLeft);
        }
        alt = cmap->cost[u] + terrain_cost(map, u, downLeft, t);
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
	for(int i = 0; i < 80*21; i++){
		printf("%d ", c->cost[i]%100);
	}

}

void character_print(character_t* c){
    printf("Character: (%d,%d)\n", c->map_x, c->map_y);
}
void character_move(character_t* c, map_t* m){
		int next = tile_id(c->map_x, c->map_y);
	switch(c->type){
		case RIVAL:
			next = m->rivalMap->prev[next];
			break;
		case HIKER:
			next = m->hikerMap->prev[next];
			break;
		case PLAYER:
			break;
		case PACER:
		case EXPLORER:
			if(c->start_dir == 1 && terrain_cost(m, next, next+1, PACER) < 100){
				next = next + 1;
			}	else if (c->start_dir == -1 && terrain_cost(m, next, next-1, PACER) < 100){
				next = next - 1;
			
			} else {
				c->start_dir *= -1;
			}
			break;
		case WANDERER:
			if(c->start_dir == 1 && terrain_cost(m, next, next+1, PACER) < 100 &&
					m->terrain[tile_id_to_y(next)][tile_id_to_x(next)].type ==
					m->terrain[tile_id_to_y(next+1)][tile_id_to_x(next+1)].type) next++;
			else if(c->start_dir == -1 && terrain_cost(m, next, next-1, PACER) < 100 &&
					m->terrain[tile_id_to_y(next)][tile_id_to_x(next)].type ==
					m->terrain[tile_id_to_y(next-1)][tile_id_to_x(next-1)].type) next--;
			else c->start_dir *= -1;	
			break;
		default:
			break;
	}	
	if(m->terrain[tile_id_to_y(next)][tile_id_to_x(next)].character == NONE){
		m->terrain[tile_id_to_y(next)][tile_id_to_x(next)].character = c->type;
		m->terrain[c->map_y][c->map_x].character = NONE;
		c->map_y = tile_id_to_y(next);
		c->map_x = tile_id_to_x(next);
	}	



}
