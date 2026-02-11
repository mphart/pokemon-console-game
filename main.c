#include "terrain.h"

#include <stdio.h>

int main(int argc, char **argv){
	printf("Creating map of size %dx%d\n", MAP_W, MAP_H);

	char* map = gen_terrain();
	print_terrain(map);

	destroy_terrain(map);

	return 0;
}
