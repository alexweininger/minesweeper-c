// do not edit
#include "cell.h"
#include "load.h"
#include "minesweep.h"
#include <stdio.h>
#include <stdlib.h>

/*
* Minesweeper game logic
* CS305 HW2 solution
* Author: Martin Cenek
* version: 1: 20180920
*/

int main(int argv, char *argc[]) {

  if (argv != 2) { 
    fprintf(stderr, "Cound not open file");
    return EXIT_FAILURE;
  }
  play_game(argc[1]);

  return EXIT_SUCCESS;
}