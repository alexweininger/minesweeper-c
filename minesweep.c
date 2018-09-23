/**
 * minesweep.c - main logic for minesweep program
 * author: Alex Weininger
 * modified: 9/23/2018
 */

#include "minesweep.h"
#include "load.h"

int play_game(char *in) {
  game *board = file_load(in);

  int gameOver = 0;

  int row = -1;
  int col = -1;
  int move = -1;

  while (!gameOver) {
    print_game(board);

    int validInput = 0;
    while (!validInput) {
      printf("Enter row in range 1-%d: enter column range 1-%d click type (0: "
             "uncover 1: mine)\n",
             board->row_max, board->col_max);
      printf("Example: 1:3 0 -> (row: 1, col: 3, move: try to uncover)\n");
      printf("Example: 2:1 1 -> (row: 2, col: 1, move: mark as a mine)\n");
      printf("Enter '-1' to quit the game.\n\n");
      scanf("%d:%d %d", &row, &col, &move);
      // printf("from command line\trow: %d\tcol: %d\tmove: %d\n", row, col,
      // move);

      // check if quit
      if (row == -1) {
        if (check_game(board)) {
          printf("YOU WIN!\nThank you for playing, have a nice day.\n");
        } else {
          printf("YOU LOSE! Mines were misidentified, have a nice day.\n");
        }
        gameOver = 1; // stop the game loop
      } else {

        // check if input is valid board location
        if (!is_safe(row - 1, col - 1, board->row_max, board->col_max)) {
          printf("\nInvalid input.\n\n");
        } else {
          validInput = 1;
        }
      }
    }

    int c = process_click(board, row, col, move);

    if (c == 0) {
      gameOver = 1;
    }

    printf("\nGood one, keep on clicking.\n");
  }

  // free allocated memory
  free_game(board);
}

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

    // if sorrounding bombs, start to uncover surrounding cells
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

        // check if surrounding cell indecies are valid before calling uncover()
        if (is_safe(newRow, newCol, board->row_max, board->col_max)) {
          uncover(board, newRow, newCol); // uncover sorrounding cell
        }
      }
    }
    return 1;
  }
}

int is_safe(int row, int col, int row_max, int col_max) {
  if (row < 0 || row >= row_max || col < 0 || col >= col_max) {
    return 0;
  } else {
    return 1;
  }
}

int uncover(game *board, int row, int col) {

  if (!is_safe(row, col, board->row_max, board->col_max)) {
    return 1;
  } else if (board->cells[row][col].mine > 0) {
    board->cells[row][col].color = white;
    return 1;
  }

  if (board->cells[row][col].color == white) {
    return 1;
  } else if (board->cells[row][col].color == black) {
    return 0;
  } else {
    // color is gray
    board->cells[row][col].color = white;

    int rowDelta[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int colDelta[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    int newRow, newCol;

    int i, unc;
    for (i = 0; i < 8; i++) {

      newRow = row + rowDelta[i];
      newCol = col + colDelta[i];

      if (is_safe(newRow, newCol, board->row_max, board->col_max)) {
        uncover(board, newRow, newCol);
      }
    }
    return unc;
  }
}

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

void print_game(game *board) {

  printf("\nBoard (%dx%d):\n", board->row_max, board->col_max);
  int i, j;
  for (i = 0; i < board->row_max; i++) {
    for (j = 0; j < board->col_max; j++) {
      if (board->cells[i][j].color == white && board->cells[i][j].mine == 0) {
        printf("0 ");
      } else if (board->cells[i][j].color == black) {
        printf("B ");
      } else if (board->cells[i][j].color == gray) {
        printf("E ");
      } else if (board->cells[i][j].color == white) {
        printf("%-d ", board->cells[i][j].mine);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void free_game(game *board) {
  int i = 0;
  for (i = 0; i < board->row_max; i++) {
    free(board->cells[i]);
  }
  free(board);
}