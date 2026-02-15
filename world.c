#include "world.h"
#include <math.h>
#include <stdlib.h>


int dist_from_center(int x, int y){
    int dx = x - WORLD_START_X;
    int dy = y - WORLD_START_Y;
    return abs(dx) + abs(dy);
}

int building_prob(int x, int y){
    return RAND_MAX*(-45.*dist_from_center(x,y)/200 + 50)/100;
}

world_t init_world(){
    return seed_world(time(NULL));
}

world_t seed_world(int seed){
    srand(seed);
    world_t* world = calloc(1, sizeof(world_t));
    world->maps = calloc(WORLD_H*WORLD_W, sizeof(map_t));
    world->seed = seed;

    world->x = WORLD_START_X;
    world->y = WORLD_START_Y;
    world->maps[WORLD_LOC(world->x, world->y)] = gen_map(0,0,0,0,building_prob(world->x,world->y));

    return *world;
}

void destroy_world(world_t world){
    for(int i = 0; i < WORLD_H*WORLD_W; i++){
        if(world.maps[i].terrain != NULL){
            destroy_map(world.maps[i]);
        }
    }
    free(world.maps);
}

void display_world(world_t w){
    print_map(w.maps[WORLD_LOC(w.x, w.y)]);
    printf("(%d,%d)\n", w.x-WORLD_START_X, w.y-WORLD_START_Y);
}

void world_move(world_t* w, int dir){
    if(dir == 0){
        // right
        w->x = w->x >= WORLD_W ? WORLD_W : w->x+1;
    } else if (dir == 1){
        // left
        w->x = w->x <= 0 ? 0 : w->x-1;
    } else if (dir == 2){
        // down
        w->y = w->y >= WORLD_H ? WORLD_H : w->y+1;
    } else if (dir == 3){
        // up
        w->y = w->y <= 0 ? 0 : w->y-1;
    } else {
        printf("Error: invalid movement key\n");
    }

    gen_map(0,0,0,0,building_prob(w->x,w->y));
}

void world_goto(world_t* w, int x, int y){
    if(x >= -WORLD_W / 2 && x <= WORLD_W / 2 && y >= -WORLD_H / 2 && y <= WORLD_H / 2){
        w->x = x+WORLD_W/2;
        w->y = y+WORLD_H/2;
        gen_map(0,0,0,0,building_prob(w->x,w->y));
    } 
}

void create_map(world_t* w, int x, int y){
    int loc = WORLD_LOC(x,y);
    if(w->maps[loc].terrain == NULL){

        int n = 0;
        if(w->maps[WORLD_LOC(x,y-1)].terrain == NULL) {
            n = 0;
        } else if(WORLD_LOC(x,y-1) < 0) {
            n = -1;
        } else {
            n = w->maps[WORLD_LOC(x,y-1)].sGate;
        }

        int s = 0;
        if(w->maps[WORLD_LOC(x,y+1)].terrain == NULL) {
            s = 0;
        } else if(WORLD_LOC(x,y+1) > WORLD_W*WORLD_H) {
            s = -1;
        } else {
            s = w->maps[WORLD_LOC(x,y+1)].nGate;
        }

        int wst = 0;
        if(w->maps[WORLD_LOC(x-1,y)].terrain == NULL) {
            wst = 0;
        } else if(x % WORLD_W == 0) {
            wst = -1;
        } else {
            wst = w->maps[WORLD_LOC(x-1,y)].eGate;
        }

        int est = 0;
        if(w->maps[WORLD_LOC(x-1,y)].terrain == NULL) {
            est = 0;
        } else if(x % WORLD_W == 0) {
            est = -1;
        } else {
            wst = w->maps[WORLD_LOC(x-1,y)].eGate;
        }



        w->maps[loc] = gen_map(0,0,0,0,building_prob(x,y));
    }
}