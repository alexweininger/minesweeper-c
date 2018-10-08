/**
 * minesweep.c - main logic for minesweep program
 * author: Alex Weininger
 * modified: 9/23/2018
 */

#include "minesweep.h"
#include "load.h"

// handles the user input and output, main function controlling the game
int play_game(char *in) {
  game *board = file_load(in); // load board from file

  if (board == NULL) { // make sure board is not NULL
    return 0;
  }

  int gameOver = 0;
  int row = -1;
  int col = -1;
  int move = -1;

  do {
    print_game(board); // print board before each turn

    int validInput = 0;
    do {

      printf("Enter row in range 1-%d: enter column range 1-%d click type (0: "
             "uncover 1: mine)\n",
             board->row_max, board->col_max);
      printf("Example: 1:3 0 -> (row: 1, col: 3, move: try to uncover)\n");
      printf("Example: 2:1 1 -> (row: 2, col: 1, move: mark as a mine)\n");
      printf("Enter '-1' to quit the game.\n\n");
      scanf("%d:%d %d", &row, &col, &move);

      // check if quit
      if (row == -1) {
        if (check_game(board)) {
          printf("YOU WIN!\nThank you for playing, have a nice day.\n");
          return 1;
        } else {
          printf("YOU LOSE! Mines were misidentified, have a nice day.\n");
          return 1;
        }
        gameOver = 1; // stop the game loop
        break;
      } else {
        // check if input is valid board location
        if (!is_safe(row - 1, col - 1, board->row_max, board->col_max)) {
          printf("\nInvalid input.\n\n");
        } else {
          validInput = 1; // input is valid
        }

        int c = process_click(board, row, col, move);

        if (c == 0) {
          gameOver = 1;
        } else {
          printf("\nGood one, keep on clicking.\n");
        }
      }
    } while (!validInput); // repeat if input is invalid

  } while (!gameOver); // stop playing when game is over

  free_game(board); // free allocated memory
}

// function that handles the clicks on the board
int process_click(game *board, int row, int col, int move) {

  // subtract 1 from row and col to convert into 0 indexed values
  row--;
  col--;

  // check if click is within the bounds of the board
  if (!is_safe(row, col, board->row_max, board->col_max)) {
    printf("Invalid input.\n");
    return 0;
  }

  cell *c = &board->cells[row][col];

  if (move == 1) {
    // if move is mark bomb, then set cell color to black
    c->color = black;
  } else if (c->mine < 0) {
    // if uncover cell with mine, game over!
    printf("\nGAME OVER! You clicked a bomb!\n");
    return 0;
  } else if (c->color == gray) {
    // uncovering gray cell, set to white
    c->color = white;

    // if surrounding bombs, start to uncover surrounding cells
    if (c->mine == 0) {
      // delta values to check adjacent cells
      int newRow, newCol;
      int rowDelta[] = {-1, 0, 1, -1, 1, -1, 0, 1};
      int colDelta[] = {-1, -1, -1, 0, 0, 1, 1, 1};

      // iterate through each delta x, y pair
      int i = 0;
      for (i = 0; i < 8; i++) {

        newRow = row + rowDelta[i];
        newCol = col + colDelta[i];

        // check if surrounding cell indicies are valid before calling uncover()
        if (is_safe(newRow, newCol, board->row_max, board->col_max)) {
          uncover(board, newRow, newCol); // uncover surrounding cell
        }
      }
    }
    return 1;
  }
}

// check wether or not an index is in array bounds
int is_safe(int row, int col, int row_max, int col_max) {
  if (row < 0 || row >= row_max || col < 0 || col >= col_max) {
    return 0;
  }
  return 1;
}

// uncover the gray cells, and any surrounding gray cells recursively.
int uncover(game *board, int row, int col) {
  if (!is_safe(row, col, board->row_max, board->col_max)) {
    return 0;
  }

  if (board->cells[row][col].mine > 0) {
    // if cell has a bomb count return 1 and turn the cell white
    board->cells[row][col].color = white;
    return 1;
  }

 if (board->cells[row][col].color == white) {
    // cell is already white, return 1
    return 1;
  } else if (board->cells[row][col].color == black) { // found bomb, return 0
    return 0;
  } else { // color is gray
    board->cells[row][col].color = white;

    int rowDelta[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int colDelta[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    int i, newRow, newCol;
    for (i = 0; i < 8; i++) {

      newRow = row + rowDelta[i];
      newCol = col + colDelta[i];
      uncover(board, newRow, newCol);
    }
  }
}

// check if user has won or lost
int check_game(game *board) {
  int i, j;
  for (i = 0; i < board->row_max; i++) {
    for (j = 0; j < board->col_max; j++) {

      if (board->cells[i][j].mine < 0) {

        if (board->cells[i][j].color != black) {
          return 0;
        }
      } else if (board->cells[i][j].color == black) {
        return 0;
      }
    }
  }
  return 1;
}

// print the board to the console
void print_game(game *board) {

  printf("\nBoard (%dx%d):\n", board->row_max, board->col_max);
  int i, j;
  for (i = 0; i < board->row_max; i++) {
    for (j = 0; j < board->col_max; j++) {
      cell *c = &board->cells[i][j];
      if (c->color == white && c->mine == 0) {
        printf("0 ");
      } else if (c->color == black) {
        printf("B ");
      } else if (c->color == gray) {
        printf("E ");
      } else if (c->color == white) {
        printf("%-d ", c->mine);
      }
    }
    printf("\n");
  }
  printf("\n");
}

// free memory allocated on the heap
void free_game(game *board) {
  int i = 0;
  for (i = 0; i < board->row_max; i++) {
    // free each row
    free(board->cells[i]);
  }
  free(board->cells); // free cells
  free(board); // free board
}