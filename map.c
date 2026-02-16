#include "map.h"

int tile_id(int x, int y){
	return x + y*MAP_W;
}
int tile_up(int id){
	return id - MAP_W;
}
int tile_right(int id){
	return id + 1;
}
int tile_left(int id){
	return id - 1;
}
int tile_down(int id){
	return id + MAP_W;
}
int tile_id_to_x(int id){
	return id % MAP_W;
}
int tile_id_to_y(int id){
	return id / MAP_W;
}

void flood_fill(map_t* map, int x, int y, Biome type, int depth){
	if(depth < 0){
		return;
	} else {
		if(map->terrain[y][x].type == UNASSIGNED) map->terrain[y][x].type = type;
		if(x-1 >= 0)    flood_fill(map, x-1, y, type, depth-1);
		if(x+1 < MAP_W) flood_fill(map, x+1, y, type, depth-1);
		if(y-1 >= 0)    flood_fill(map, x, y-1, type, depth-1);
		if(y+1 < MAP_H) flood_fill(map, x, y+1, type, depth-1);
	}
}

Biome random_biome(){
	int randint = rand() % NUM_BIOMES;
	if(randint == 0){
		return BOULDER;
	} else if(randint == 1){
		return POND;
	} else if(randint == 2){
		return PRAIRIE;
	} else if(randint == 3){
		return CLEARING;
	} else if(randint == 4){
		return FOREST;
	} 
	return PRAIRIE;
}

bool is_filled(map_t* map){
	for(int h = 0; h < MAP_H; h++){
		for(int w = 0; w < MAP_W; w++){
			if(map->terrain[h][w].type == UNASSIGNED){
				return false;
			}
		}
	}
	return true;
}

bool place_single(map_t* map, Biome type){
	int x = rand()%MAP_W;
	int y = rand()%MAP_H;
	if(map->terrain[y][x].type == BOULDER || map->terrain[y][x].type == POND){
		return false;
	} else {
		map->terrain[y][x].type = type;
		return true;
	}
}

int cost(map_t* map, int u, int v){
	int cost = 1;
	Biome uType = map->terrain[tile_id_to_y(u)][tile_id_to_x(u)].type;
	Biome vType = map->terrain[tile_id_to_y(v)][tile_id_to_x(v)].type;
	if(uType != vType){
		cost += 2;
	}
	int vX = tile_id_to_x(v);
	int vY = tile_id_to_y(v);
	if(vX < 5){
		cost += 2;
	} else if(MAP_W - vX < 5){
		cost += 2;
	}
	if(vY < 5){
		cost += 2;
	} else if(MAP_H - vY < 5){
		cost += 2;
	}
	switch(vType){
		case BORDER:
			cost = 9999;
			break;
		case BOULDER:
			cost += 4;
			break;
		case POND:
			cost += 5;
			break;
		case FOREST:
			cost += 2;
			break;
		case CLEARING:
			cost += 1;
			break;
		case PRAIRIE:
			cost += 1;
			break;
		case PATH:
			cost += 1;
			break;
	}
	return cost;
}

int place_path(map_t* map, int s, int t){
	int* dist = calloc(MAP_W*MAP_H, sizeof(int));
	int* prev = calloc(MAP_W*MAP_H, sizeof(int));
	heap_t* h = heap_init(MAP_W*MAP_H);
	for(int i = 0; i < MAP_W*MAP_H; i++){
		dist[i] = i == s ? 0 : INFINITY;
		prev[i] = -1;
		heap_insert(h, dist[i], i);
	}

	while(h->size > 0){
		int u = heap_removeMin(h);

		int up = tile_up(u);
		int down = tile_down(u);
		int right = tile_right(u);
		int left = tile_left(u);

		int alt = dist[u] + cost(map, u, up);
		if(up >= 0 && alt < dist[up]){
			dist[up] = alt;
			prev[up] = u;
			heap_updatePriority(h,dist[up],up);
		}
		alt = dist[u] + cost(map, u, down);
		if(down < MAP_H*MAP_W && alt < dist[down]){
			dist[down] = alt;
			prev[down] = u;
			heap_updatePriority(h,dist[down],down);
		}
		alt = dist[u] + cost(map, u, right);
		if(tile_id_to_x(right) > 0 && alt < dist[right]){
			dist[right] = alt;
			prev[right] = u;
			heap_updatePriority(h,dist[right],right);
		}
		alt = dist[u] + cost(map, u, left);
		if(tile_id_to_x(left) < MAP_W - 1 && alt < dist[left]){
			dist[left] = alt;
			prev[left] = u;
			heap_updatePriority(h,dist[left],left);
		}
	}

	if(prev[t] == -1){
		printf("fatal error: impossible to draw a path from %d to %d\n", s, t);
	} else {
		// printf("Cost to get from s(%d) to t(%d): %d\n", s,t,dist[t]);
		t = prev[t];
		while(t != s){
			map->terrain[tile_id_to_y(t)][tile_id_to_x(t)].type = PATH;
			t = prev[t];
		}
	}

	free(dist);
	free(prev);
	heap_destroy(h);
	return 0;
}

bool place_2by2(map_t* map, int sX, int sY, Biome type){
	if(sY <= 0) return false;
	if(sY+1 >= MAP_H-1) return false;
	if(map->terrain[sY][sX].type == BORDER || map->terrain[sY][sX].type == PATH|| map->terrain[sY][sX].type == GATE) return false;
	if(map->terrain[sY+1][sX].type == BORDER || map->terrain[sY+1][sX].type == PATH|| map->terrain[sY][sX].type == GATE) return false;
	if(map->terrain[sY][sX+1].type == BORDER || map->terrain[sY][sX+1].type == PATH|| map->terrain[sY][sX].type == GATE) return false;
	if(map->terrain[sY+1][sX+1].type == BORDER || map->terrain[sY+1][sX+1].type == PATH|| map->terrain[sY][sX].type == GATE) return false;
	map->terrain[sY][sX].type = type;
	map->terrain[sY+1][sX].type = type;
	map->terrain[sY][sX+1].type = type;
	map->terrain[sY+1][sX+1].type = type;
	return true;
}

bool place_building(map_t* map, Biome type){
	int startX = BUILDING_X_START + rand()%BUILDING_X_RANGE;
	int startY = -1;
	for(int i = 1; i < MAP_H; i++){
		if(map->terrain[i][startX].type == PATH){
			startY = i;
			break;
		}
	}
	if          (place_2by2(map, startX-2, startY-2, type)) return true;
	else if     (place_2by2(map, startX-2, startY+1, type)) return true;
	else if     (place_2by2(map, startX+1, startY-2, type)) return true;
	else return place_2by2(map, startX+1, startY+1, type);
}

map_t* gen_map(int n, int s, int e, int w, int buildingChance){
	map_t* map = calloc(1, sizeof(map_t));
	map->generated = true;

	// place seeds randomly in 8 octrants of the map
	int numOctrants = 8;
	Biome octrants[] = {PRAIRIE, FOREST, CLEARING, POND, UNASSIGNED, PRAIRIE, UNASSIGNED, CLEARING};
	int startOctrant = rand() % numOctrants;
	for(int i = 0; i < numOctrants; i++){
		int row = i%2;
		int col = i/2;
		int xStart = col * (MAP_W/4);
		int xRange = MAP_W/4;
		int yStart = row * (MAP_H/2);
		int yRange = MAP_H/2;
		int seedX = xStart + rand() % xRange;
		int seedY = yStart + rand() % yRange;
		int octrantIndex = (startOctrant + i) % numOctrants;
		Biome seed = octrants[octrantIndex] == UNASSIGNED ? random_biome() : octrants[octrantIndex];
		map->terrain[seedY][seedX].type = seed;
	}

	// fill the terrain
	while(!is_filled(map)){
		int x = rand()%MAP_W;
		int y = rand()%MAP_H;
		Biome type = map->terrain[y][x].type;
		flood_fill(map, x, y, type, 1);
	}

	// place random trees and boulders
	for(int i = 0; i < NUM_SINGLES; i++){
		place_single(map, random_biome());
	}

	// place border
	for(int i = 0; i < MAP_W; i++){
		map->terrain[0][i].type = BORDER;
		map->terrain[MAP_H-1][i].type = BORDER;
	}
	for(int i = 1; i < MAP_H - 1; i++){
		map->terrain[i][0].type = BORDER;
		map->terrain[i][MAP_W-1].type = BORDER;
	}

	// place paths
	bool fillN = n < 0;
	bool fillS = s < 0;
	bool fillW = w < 0;
	bool fillE = e < 0;

	int nsPathStart = n > 0 ? n : PATH_X_START + rand()%PATH_X_RANGE;
	int nsPathEnd =   s > 0 ? s : PATH_X_START + rand()%PATH_X_RANGE;
	int wePathStart = w > 0 ? w : PATH_Y_START + rand()%PATH_Y_RANGE;
	int wePathEnd =   e > 0 ? e : PATH_Y_START + rand()%PATH_Y_RANGE;

	map->nGate = nsPathStart;
	map->sGate = nsPathEnd;
	map->wGate = wePathStart;
	map->eGate = wePathEnd;

	if(!fillN) map->terrain[0][nsPathStart].type = GATE;
	if(!fillS) map->terrain[MAP_H-1][nsPathEnd].type = GATE;
	if(!fillW) map->terrain[wePathStart][0].type = GATE;
	if(!fillE) map->terrain[wePathEnd][MAP_W-1].type = GATE;

	place_path(map, tile_id(nsPathStart,0), tile_id(nsPathEnd,MAP_H-1));
	place_path(map, tile_id(0,wePathStart), tile_id(MAP_W-1,wePathEnd));

	// place pokecenter and pokemart
	printf("building chance: %d\n", buildingChance);
	bool placed = false;
	if(rand() < buildingChance){
		do{placed = place_building(map, POKEMART);} while(!placed);
	}
	placed = false;
	if(rand() < buildingChance){
		do{placed = place_building(map, POKECENTER);} while(!placed);
	}

	return map;
}

void destroy_map(map_t* m){
	free(m->terrain);
	free(m);
}

int print_map(map_t* map){
	for(int h = 0; h < MAP_H; h++){
		for(int w = 0; w < MAP_W; w++){
			Biome t = map->terrain[h][w].type;
			switch (t)
			{
			case BORDER:
				printf("%c",'%');
				break;
			case BOULDER:
				printf("%c",'%');
				break;
			case PRAIRIE:
				printf("%c",':');
				break;
			case CLEARING:
				printf("%c",'.');
				break;
			case POND:
				printf("%c",'~');
				break;
			case FOREST:
				printf("%c",'^');
				break;
			case PATH:
				printf("%c",'#');
				break;
			case GATE:
				printf("%c",'#');
				break;
			case POKEMART:
				printf("%c",'M');
				break;
			case POKECENTER:
				printf("%c",'C');
				break;
			case UNASSIGNED:
				printf("%c",' ');
				break;
			default:
				printf("\nUnknown type encountered when printing the map\n");
				return 1;
			}
			Character c = map->terrain[h][w].character;
		}
		printf("\n");
	}
	return 0;
}