#include <stdio.h>

#include "board.h"

Move get_user_move() {
  char col_from, col_to;
  int row_from, row_to;

  printf("Enter your move: ");

  while (true) {
    if (scanf("%c%d %c%d", &col_from, &row_from, &col_to, &row_to) == 4) {
      break;
    }
  }

  return (Move){.from = {.x = col_from - 'A', .y = row_from - 1},
                .to = {.x = col_to - 'A', .y = row_to - 1}};
}

int main(void) {
  Board board = {};
  init_board(&board);
  print_board(&board);

  printf("Turn: %s\n", board.white_turn ? "WHITE" : "BLACK");
  return 0;
}
