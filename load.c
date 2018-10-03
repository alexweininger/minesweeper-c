/**
 * load.c - loads board from file
 * author: Alex Weininger
 * modified: 9/23/2018
 */

#include "load.h"
#include "cell.h"
#include "minesweep.h"

game *file_load(char *filename) {
  game *board = (game *)malloc(sizeof(game));

  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Error: could not open file: %s for reading.\n", filename);
    return NULL;
  }

  // read numRows/numCols from file
  int numRows = 0;
  int numCols = 0;
  fscanf(fp, "%d %d", &numRows, &numCols);

  if (numRows < 3 || numCols < 3) {
    printf("invalid board: rows=%d, cols=%d\n", numRows, numCols);
    return NULL;
  }

  board->row_max = numRows;
  board->col_max = numCols;

  board->cells = (cell **)calloc(numRows, sizeof(cell **));

  int i;
  for (i = 0; i < numRows; i++) {
    board->cells[i] = (cell *)calloc(numCols, sizeof(cell *));
    board->cells[i]->color = gray;
  }
  int mineNumber = 0;
  int * mineNumber_ptr = &mineNumber;
  for (i = 0; i < numRows; i++) {
    int j;
    for (j = 0; j < numCols; j++) {
      fscanf(fp, "%d", mineNumber_ptr);
      if (mineNumber > -2 && mineNumber < 9) {
        board->cells[i][j].mine;
      } else {
        return NULL;
      }
    }
    fscanf(fp, "\n");
  }
  fclose(fp);
  return board;
}