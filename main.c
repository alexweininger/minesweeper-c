// do not edit
#include <stdlib.h>
#include <stdio.h>
#include "load.h"
#include "cell.h"
#include "minesweep.h"

/*
* Minesweeper game logic
* CS305 HW2 solution
* Author: Martin Cenek
* version: 1: 20180920 
*/

int main(int argv, char* argc[]){

	if(argv != 2){
		fprintf(stderr, "Cound not open file");
		return EXIT_FAILURE;
	}
	play_game(argc[1]);

	return EXIT_SUCCESS;
}