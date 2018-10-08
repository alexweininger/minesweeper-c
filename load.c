/**
 * load.c - loads board from file
 * author: Alex Weininger
 * modified: 9/23/2018
 */

#include "load.h"
#include "cell.h"
#include "minesweep.h"

game *file_load(char *filename) {

  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Error: Could not open file \"%s\" for reading.\n", filename);
    return NULL;
  }

  // read numRows and numCols from file
  int numRows = 0;
  int numCols = 0;
  fscanf(fp, "%d %d", &numRows, &numCols);

  if (numRows < 3 || numCols < 3) {
    printf("Error: Invalid dimensions rows: %d\tcols: %d\n", numRows, numCols);
    return NULL;
  }

  game *board = (game *)malloc(sizeof(game));

  board->row_max = numRows;
  board->col_max = numCols;

  board->cells = (cell **)calloc(numRows, sizeof(cell **));

  int i;
  for (i = 0; i < numRows; i++) {
    board->cells[i] = (cell *)calloc(numCols, sizeof(cell *));
    board->cells[i]->color = gray;
  }
  int mineNumber = 0;
  // int * mineNumber_ptr = &mineNumber;
  for (i = 0; i < numRows; i++) { // go through each row
    int j;
    for (j = 0; j < numCols; j++) { // each col
      fscanf(fp, "%d", &mineNumber);
      if (mineNumber > -2 && mineNumber < 9) { // check mine number for error
        board->cells[i][j].mine = mineNumber;
      } else {
        printf("Error: Invalid board value %d, %d: %d.\n", j + 1, i + 1, mineNumber);
        return NULL;
      }
    }
    fscanf(fp, "\n"); // get the newline
  }
  fclose(fp); // close file
  return board;
}