#include "world.h"


int clamp(int n, int min, int max){
    if(n < min) return min;
    if(n > max) return max;
    return n;
}

int dist_from_center(int x, int y){
    int dx = abs(x - WORLD_START_X);
    int dy = abs(y - WORLD_START_Y);
    return dx > dy ? dx : dy;
}

int building_prob(int x, int y){
    double d = dist_from_center(x,y);
    double num = WORLD_W - d;
    double den = WORLD_W;
    return RAND_MAX * (num/den) / 2;
}

world_t* init_world(int numTrainers){
    return seed_world(numTrainers, time(NULL));
}

world_t* seed_world(int numTrainers, int seed){
    srand(seed);
    world_t* world = calloc(1, sizeof(world_t));
    world->seed = seed;

    world->x = WORLD_START_X;
    world->y = WORLD_START_Y;
    world->numTrainers = numTrainers;

    world->maps = calloc(WORLD_H, sizeof(map_t));
    for(int i = 0; i < WORLD_H; i++){
        world->maps[i] = calloc(WORLD_W, sizeof(map_t));
    }

    world->player = character_init(PLAYER, -1, -1);

    world_goto(world, 0, 0);

    return world;
}

void destroy_world(world_t* world){
    // for(int i = 0; i < WORLD_H; i++){
    //     free(world->maps[i]);
    // }
    free(world->maps);

    free(world->player);

    free(world);
}

void display_world(world_t* w){
    print_map(&w->maps[w->y][w->x]);
    printf("(%d,%d)\n", w->x-WORLD_W/2, w->y-WORLD_H/2);
}

void world_move(world_t* w, Direction d){
    switch(d){
        case NORTH:
            world_goto(w, w->x-WORLD_W/2, w->y-WORLD_H/2-1);
            break;
        case EAST:
            world_goto(w, w->x-WORLD_W/2+1, w->y-WORLD_H/2);
            break;
        case SOUTH:
            world_goto(w, w->x-WORLD_W/2, w->y-WORLD_H/2+1);
            break;
        case WEST:
            world_goto(w, w->x-WORLD_W/2-1, w->y-WORLD_H/2);
            break;
    }
}

void world_goto(world_t* w, int x, int y){
	if(w->maps[w->y][w->x].generated){
		map_removePlayer(&w->maps[w->y][w->x], w->player);
	}

    x = clamp(x, -WORLD_W/2, WORLD_W/2);
    y = clamp(y, -WORLD_H/2, WORLD_H/2);
    w->x = x + WORLD_W/2;
    w->y = y + WORLD_H/2;
    if(!w->maps[w->y][w->x].generated){
        int nth = w->y-1 < 0 ? -1        : w->maps[w->y-1][w->x].sGate;
        int sth = w->y+1 >= WORLD_H ? -1 : w->maps[w->y+1][w->x].nGate;
        int est = w->x+1 >= WORLD_W ? -1 : w->maps[w->y][w->x+1].wGate;
        int wst = w->x-1 < 0 ? -1        : w->maps[w->y][w->x-1].eGate;

        srand(w->seed + 1 + w->y*WORLD_W+w->x);
        w->maps[w->y][w->x] = *gen_map(nth,sth,est,wst,building_prob(w->x,w->y),w->numTrainers); 
    }


    map_putPlayer(&w->maps[w->y][w->x], w->player);

    while(true){
	    for(int i = 0; i < w->numTrainers; i++){
		character_move(&w->maps[w->y][w->x].trainers[i], &w->maps[w->y][w->x]);
		usleep(250000);
		
    display_world(w);
	}}
}
