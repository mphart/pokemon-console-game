#ifndef TERRAIN_H
#define TERRAIN_H

#define MAP_W 80
#define MAP_H 21

#define TILE_UP(x) x - MAP_W
#define TILE_RIGHT(x) x + 1
#define TILE_LEFT(x) x - 1
#define TILE_DOWN(x) x + MAP_W

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

char* gen_terrain();
char* seed_terrain(int seed);
void print_terrain(char* terrain);
void destroy_terrain(char* terrain);

#endif
