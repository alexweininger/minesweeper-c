#include "minesweep.h"
#include "load.h"

int play_game(char *in) {
  game *board = file_load(in);
  print_game(board);
}

void print_game(game *board) {

  int i, j;
  for (i = 0; i < board->row_max; i++) {
    for (j = 0; j < board->col_max; j++) {

      printf("%2d\t", board->cells[i][j].mine);
    }

    printf("\n");
  }
}