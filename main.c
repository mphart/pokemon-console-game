#include "world.h"

#include <stdio.h>

int main(int argc, char **argv){

	world_t world = init_world();
	
	printf("Welcome to Pokemon! Check out the following commands to move:\n n : go north\n s : go south\n w : go east\n e : go west\n");
	display_world(world);

	char cmd = ' ';
	do {
		cmd = getchar();
		
		if(cmd == 'e'){
			world_move(&world, 0);
			display_world(world);
		} else if(cmd == 'w'){
			world_move(&world, 1);
			display_world(world);
		} else if(cmd == 's'){
			world_move(&world, 2);
			display_world(world);
		} else if(cmd == 'n'){
			world_move(&world, 3);
			display_world(world);
		} else if(cmd == 'f'){
			int x;
			int y;
			scanf("%d",&x);
			scanf("%d",&y);
			world_goto(&world, x, y);
			display_world(world);
		}

	}while(cmd != 'q');

	destroy_world(world);

	return 0;
}

void print_error(){
	printf("Please enter a valid command\n");
}
