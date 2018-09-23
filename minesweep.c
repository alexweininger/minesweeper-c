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
      printf("Game over! You lose!\n");
      gameOver = 1;
    }
  }
}

int process_click(game *board, int x, int y, int move) {
  x--;
  y--;

  cell *c = &board->cells[x][y];

  if (move == 1) {
    c->color = black;
  } else if (c->color == gray && c->mine < 0) {
    c->color = white;
  } else if (c->mine == 1) {
    printf("Game Over\n");
    return 0;
  } else {

    int adjx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int adjy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    int x1, y1;

    int i = 0;
    for (i = 0; i < 8; i++) {

      for (int j = 0; j < 8; j++) {
        x1 = x + adjx[j];
        y1 = y + adjy[j];

        if (is_safe(x1, y1, board->row_max, board->col_max)) {
          return uncover(board, x1, y1);
        }
      }
    }
  }
}

int is_safe(int row, int col, int row_max, int col_max) {
  if (row < 0 || row > row_max || col < 0 || col > col_max) {
    return 0;
  } else {
    return 1;
  }
}

int uncover(game *board, int row, int col) {

  if (board->cells[row][col].color == black) {
    return 0;
  } else if (board->cells[row][col].color == gray) {
    // TODO
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
      if (board->cells[i][j].color == white && board->cells[i][j].mine < 2) {
        printf("0 ");
      } else if (board->cells[i][j].color == black) {
        printf("B ");
      } else if (board->cells[i][j].color == gray) {
        printf("E ");
      } else if (board->cells[i][j].color == white) {
        printf("%1d", board->cells[i][j].mine);
      }
    }

    printf("\n");
  }
  printf("\n");
}