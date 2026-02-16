#include "world.h"
#include <math.h>
#include <stdlib.h>

int clamp(int n, int min, int max){
    if(n < min) return min;
    if(n > max) return max;
    return n;
}

int dist_from_center(int x, int y){
    int dx = x - WORLD_START_X;
    int dy = y - WORLD_START_Y;
    return abs(dx) + abs(dy);
}

int building_prob(int x, int y){
    return RAND_MAX*(-45.*dist_from_center(x,y)/200 + 50)/100;
}

world_t* init_world(){
    return seed_world(time(NULL));
}

world_t* seed_world(int seed){
    srand(seed);
    world_t* world = calloc(1, sizeof(world_t));
    world->seed = seed;

    world->x = WORLD_START_X;
    world->y = WORLD_START_Y;

    world->maps = calloc(WORLD_H, sizeof(map_t));
    for(int i = 0; i < WORLD_H; i++){
        world->maps[i] = calloc(WORLD_W, sizeof(map_t));
    }
    world->maps[world->y][world->x] = *gen_map(0,0,0,0,building_prob(world->x,world->y));

    return world;
}

void destroy_world(world_t* world){
    // for(int i = 0; i < WORLD_H; i++){
    //     free(world->maps[i]);
    // }
    free(world->maps);
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
    x = clamp(x, -WORLD_W/2, WORLD_W/2);
    y = clamp(y, -WORLD_H/2, WORLD_H/2);
    w->x = x + WORLD_W/2;
    w->y = y + WORLD_H/2;
    if(!w->maps[w->y][w->x].generated){
        int nth = w->y-1 < 0 ? -1        : w->maps[w->y-1][w->x].sGate;
        int sth = w->y+1 >= WORLD_H ? -1 : w->maps[w->y+1][w->x].nGate;
        int est = w->x+1 >= WORLD_W ? -1 : w->maps[w->y][w->x+1].wGate;
        int wst = w->x-1 < 0 ? -1        : w->maps[w->y][w->x-1].eGate;
        w->maps[w->y][w->x] = *gen_map(nth,sth,est,wst,building_prob(w->x,w->y));
    }
}