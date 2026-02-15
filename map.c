#include "map.h"


bool is_complete(char* trn){
	for(int i = 0; i < MAP_W*MAP_H; i++){
		if(trn[i] == ' ') return false;
	}
	return true;
}

int place_seed(char* terrain, char type, bool overrideExisting){
	int loc = -1;
	do{
		loc = rand() % (MAP_W*MAP_H);
	} while(!overrideExisting && terrain[loc] != ' ');
	terrain[loc] = type;
	return loc;
}

void flood_fill(char* terrain, int loc, int depth){
	if(depth > 0 && terrain[loc] != ' '){
		if(loc - MAP_W >= 0) {
			terrain[loc-MAP_W] = terrain[loc];
			flood_fill(terrain, loc-MAP_W, depth - 1);
		}
		if(loc + MAP_W < MAP_W*MAP_H) {
			terrain[loc+MAP_W] = terrain[loc];
			flood_fill(terrain, loc+MAP_W, depth - 1);
		}
		if(loc % MAP_W > 0){
			terrain[loc-1] = terrain[loc];
			flood_fill(terrain, loc-1, depth - 1);
		}
		if(loc % MAP_W < MAP_W - 1) {
			terrain[loc+1] = terrain[loc];
			flood_fill(terrain, loc-1, depth - 1);
		}
	}
}

int place_2by2(char* terrain, char type){
	int loc = -1;
	do{
		loc = 2*MAP_W + rand()%(MAP_H*MAP_W - 4*MAP_W);
	} while(terrain[loc] != '#' || loc%MAP_W <= 2 || loc%MAP_W >= MAP_W-3);

	if(terrain[TILE_UP(loc)] != '#' && terrain[TILE_UP(loc)] != 'C' && terrain[TILE_UP(loc)] != 'M')
	{
		terrain[TILE_UP(loc)] = type;
		if(terrain[TILE_UP(TILE_UP(loc))] != '#') terrain[TILE_UP(TILE_UP(loc))] = type;
		if(terrain[TILE_RIGHT(TILE_UP(loc))] != '#') terrain[TILE_RIGHT(TILE_UP(loc))] = type;
		if(terrain[TILE_RIGHT(TILE_UP(TILE_UP(loc)))] != '#') terrain[TILE_RIGHT(TILE_UP(TILE_UP(loc)))] = type;
	} 
	else if(terrain[TILE_DOWN(loc)] != '#' && terrain[TILE_DOWN(loc)] != 'C' && terrain[TILE_DOWN(loc)] != 'M')
	{
		terrain[TILE_DOWN(loc)] = type;
		if(terrain[TILE_DOWN(TILE_DOWN(loc))] != '#') terrain[TILE_DOWN(TILE_DOWN(loc))] = type;
		if(terrain[TILE_RIGHT(TILE_DOWN(loc))] != '#') terrain[TILE_RIGHT(TILE_DOWN(loc))] = type;
		if(terrain[TILE_RIGHT(TILE_DOWN(TILE_DOWN(loc)))] != '#') terrain[TILE_RIGHT(TILE_DOWN(TILE_DOWN(loc)))] = type;
	} 
	else if(terrain[TILE_RIGHT(loc)] != '#' && terrain[TILE_RIGHT(loc)] != 'C' && terrain[TILE_RIGHT(loc)] != 'M')
	{
		terrain[TILE_RIGHT(loc)] = type;
		if(terrain[TILE_DOWN(TILE_RIGHT(loc))] != '#') terrain[TILE_DOWN(TILE_RIGHT(loc))] = type;
		if(terrain[TILE_RIGHT(TILE_RIGHT(loc))] != '#') terrain[TILE_RIGHT(TILE_RIGHT(loc))] = type;
		if(terrain[TILE_DOWN(TILE_RIGHT(TILE_RIGHT(loc)))] != '#') terrain[TILE_DOWN(TILE_RIGHT(TILE_RIGHT(loc)))] = type;
	} 
	else if(terrain[TILE_LEFT(loc)] != '#' && terrain[TILE_LEFT(loc)] != 'C' && terrain[TILE_LEFT(loc)] != 'M')
	{
		terrain[TILE_LEFT(loc)] = type;
		if(terrain[TILE_DOWN(TILE_LEFT(loc))] != '#') terrain[TILE_DOWN(TILE_LEFT(loc))] = type;
		if(terrain[TILE_LEFT(TILE_LEFT(loc))] != '#') terrain[TILE_LEFT(TILE_LEFT(loc))] = type;
		if(terrain[TILE_DOWN(TILE_LEFT(TILE_LEFT(loc)))] != '#') terrain[TILE_DOWN(TILE_LEFT(TILE_LEFT(loc)))] = type;
	} 
	else {
		place_2by2(terrain, type);
	}
	return loc;
}

map_t gen_map(int n, int s, int e, int w, int buildingChance){
	map_t map;
	map.terrain = calloc(MAP_W*MAP_H, sizeof(char));
	for(int i = 0; i < MAP_W*MAP_H; i++){
        map.terrain[i] = ' ';
    }

	// place seeds
	int prairieCount = 2 + rand()%4;  //2;
	int clearingCount = 2 + rand()%4;  //3;
	int pondCount = 1 + rand()%2;      
	int mountainCount = 0 + rand()%3;    //2;
	int forestCount = 0 + rand()%3; 
	int* prairieSeeds = calloc(prairieCount, sizeof(int));
	int* pondSeeds = calloc(pondCount, sizeof(int));
	int* clearingSeeds = calloc(clearingCount, sizeof(int));
	int* mountainSeeds = calloc(mountainCount, sizeof(int));
	int* forestSeeds = calloc(forestCount, sizeof(int));
	for(int i = 0; i < prairieCount; i++){
		prairieSeeds[i] = place_seed(map.terrain, ':', false);
	}
	for(int i = 0; i < pondCount; i++){
        pondSeeds[i] = place_seed(map.terrain, '~', false);
    }
	for(int i = 0; i < clearingCount; i++){
        clearingSeeds[i] = place_seed(map.terrain, '.', false);
    }
	for(int i = 0; i < mountainCount; i++){
        mountainSeeds[i] = place_seed(map.terrain, '%', false);
    }
	for(int i = 0; i < forestCount; i++){
        forestSeeds[i] = place_seed(map.terrain, '^', false);
    }

	// fill the terrain
	while(!is_complete(map.terrain)){
		int loc = rand()%(MAP_H*MAP_W);
		int depth = 1;
		flood_fill(map.terrain, loc, depth);
	}

	// place singles
	int numSingles = 11 + rand()%10;
	for(int i = 0; i < numSingles; i++){
		int type = rand()%10;
		if(type <= 0){
			place_seed(map.terrain, '~', true);
		} else if(type <= 5){
			place_seed(map.terrain, '%', true);
		} else {
			place_seed(map.terrain, '^', true);
		}
	}

	// place border
	for(int i = 0; i < MAP_W; i++){
		map.terrain[i] = '%';
		map.terrain[MAP_W*MAP_H-i-1] = '%';
	}
	for(int i = 0; i < MAP_H; i++){
		map.terrain[i*MAP_W] = '%';
		map.terrain[i*MAP_W+MAP_W-1] = '%';
	}

	// place paths
	int nsPathIndex = 6 + rand()%50;
	int sPathIndex = nsPathIndex + rand()%20;
	if(n != 0) nsPathIndex = n;
	if(s != 0) sPathIndex = s;
	map.nGate = nsPathIndex;
	map.sGate = sPathIndex;
	int northStart = n < 0 ? 1 : 0;
	int southEnd = s < 0 ? MAP_H - 1 : MAP_H;
	for(int i = northStart; i < southEnd; i++){
		map.terrain[nsPathIndex + i*MAP_W] = '#';
		if(nsPathIndex != sPathIndex){
			if(rand()%6 == 0){
				int delta = rand()%(sPathIndex - nsPathIndex);
				for(int k = 0; k < delta; k++){
					nsPathIndex += 1;
					map.terrain[nsPathIndex + i*MAP_W] = '#';
				}
			}
		}
	}
	int wePathIndex = (4 + rand()%14)*MAP_W;
	int ePathIndex = (wePathIndex + rand()%3)*MAP_W;
	if(e != 0) ePathIndex = e;
	if(w != 0) wePathIndex = w;
	map.wGate = wePathIndex;
	map.eGate = ePathIndex;
	int leftStart = w < 0 ? 1 : 0;
	int leftEnd = e < 0 ? MAP_W - 1 : MAP_W;
	for(int i = leftStart; i < leftEnd; i++){
		map.terrain[wePathIndex + i] = '#';
		// if(wePathIndex != ePathIndex){
		// 	if(rand()%6 == 0){
		// 		int delta = rand()%(ePathIndex - wePathIndex);
		// 		for(int k = 0; k < delta; k++){
		// 			wePathIndex += 1;
		// 			map.terrain[wePathIndex + i] = '#';
		// 		}
		// 	}
		// }
	}

	// place pokecenter and pokemart
	if(rand() < buildingChance){
		place_2by2(map.terrain, 'C');
	}
	if(rand() < buildingChance){
		place_2by2(map.terrain, 'M');
	}

	return map;
}

void print_map(map_t map){
	for(int h = 0; h < MAP_H; h++){
		for(int w = 0; w < MAP_W; w++){
			printf("%c", map.terrain[h*MAP_W+w]);
		}
		printf("\n");
	}
}


void destroy_map(map_t map){
	free(map.terrain);
}