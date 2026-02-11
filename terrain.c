#include "terrain.h"


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

char* seed_terrain(int seed){
	char* terrain = calloc(MAP_W*MAP_H, sizeof(char));
	for(int i = 0; i < MAP_W*MAP_H; i++){
        terrain[i] = ' ';
    }

	srand(seed);

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
		prairieSeeds[i] = place_seed(terrain, ':', false);
	}
	for(int i = 0; i < pondCount; i++){
        pondSeeds[i] = place_seed(terrain, '~', false);
    }
	for(int i = 0; i < clearingCount; i++){
        clearingSeeds[i] = place_seed(terrain, '.', false);
    }
	for(int i = 0; i < mountainCount; i++){
        mountainSeeds[i] = place_seed(terrain, '%', false);
    }
	for(int i = 0; i < forestCount; i++){
        forestSeeds[i] = place_seed(terrain, '^', false);
    }

	// fill the terrain
	while(!is_complete(terrain)){
		int loc = rand()%(MAP_H*MAP_W);
		int depth = 1;
		flood_fill(terrain, loc, depth);
	}

	// place singles
	int numSingles = 11 + rand()%10;
	for(int i = 0; i < numSingles; i++){
		int type = rand()%10;
		if(type <= 0){
			place_seed(terrain, '~', true);
		} else if(type <= 5){
			place_seed(terrain, '%', true);
		} else {
			place_seed(terrain, '^', true);
		}
	}

	// place border
	for(int i = 0; i < MAP_W; i++){
		terrain[i] = '%';
		terrain[MAP_W*MAP_H-i-1] = '%';
	}
	for(int i = 0; i < MAP_H; i++){
		terrain[i*MAP_W] = '%';
		terrain[i*MAP_W+MAP_W-1] = '%';
	}

	// place paths
	int nsPathIndex = 5 + rand()%70;
	for(int i = 0; i < MAP_H; i++){
		terrain[nsPathIndex + i*MAP_W] = '#';
		int move = rand()%10;
		int dir = 0;
		int delta = 0;
		if(move <= 0 && nsPathIndex%MAP_W > 4){
			dir = -1;
			delta += rand()%3;
		} else if(move >= 9 && nsPathIndex%MAP_W < MAP_W-4){
			dir = 1;
			delta += rand()%3;
		}
		for(int k = 1; k <= delta; k++){
			terrain[nsPathIndex + i*MAP_W+ (dir*k)] = '#';
		}
		nsPathIndex += (dir*delta);
	}
	int wePathIndex = (3 + rand()%15)*MAP_W;
	for(int i = 0; i < MAP_W; i++){
		terrain[wePathIndex + i] = '#';
	}

	// place pokecenter and pokemart
	place_2by2(terrain, 'C');
	place_2by2(terrain, 'M');

	return terrain;
}

void print_terrain(char* trn){
	for(int h = 0; h < MAP_H; h++){
		for(int w = 0; w < MAP_W; w++){
			printf("%c", trn[h*MAP_W+w]);
		}
		printf("\n");
	}
}

char* gen_terrain(){
	return seed_terrain(time(NULL));
}

void destroy_terrain(char* terrain){
	free(terrain);
}