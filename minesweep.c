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
    printf("Enter row in range 1-%d: enter column range 1-%d click type (0: "
           "uncover 1: mine)\n",
           board->row_max, board->col_max);
    printf("Example= 1:3 0 -- row=1 col=3 try uncover\n");
    printf("Example= 2:1 1 -- row=2 col=1 mark as a mine\n");
    printf("Enter '-1' to quit the game\n");

    scanf("%d:%d %d", &row, &col, &move);
    printf("from command line\trow: %d\tcol: %d\tmove type: %d\n", row, col,
           move);

    int c = process_click(board, row, col, move);

    if (c == 0) {
      gameOver = 1;
    }
  }

  free_game(board);
}

int process_click(game *board, int row, int col, int move) {

  row--;
  col--;

  if (!is_safe(row, col, board->row_max, board->col_max)) {
    printf("Invalid input.\n");
    return 0;
  }

  cell *c = &board->cells[row][col];

  if (move == 1) {
    c->color = black;
  } else if (c->mine < 0) {
    return 0;
  } else if (c->color == gray) {

    c->color = white;

    if (c->mine == 0) {

      int rowDelta[] = {-1, 0, 1, -1, 1, -1, 0, 1};
      int colDelta[] = {-1, -1, -1, 0, 0, 1, 1, 1};

      int newRow, newCol;

      int i = 0;
      for (i = 0; i < 8; i++) {

        newRow = row + rowDelta[i];
        newCol = col + colDelta[i];

        if (is_safe(newRow, newCol, board->row_max, board->col_max)) {
          uncover(board, newRow, newCol);
        }
      }
    }
  }
}

int is_safe(int row, int col, int row_max, int col_max) {
  if (row < 0 || row >= row_max || col < 0 || col >= col_max) {
    return 0;
  } else {
    return 1;
  }
}
/*
 * TODO done
 */
int uncover(game *board, int row, int col) {
  print_game(board);

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