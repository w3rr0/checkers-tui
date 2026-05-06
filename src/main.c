#include <ctype.h>
#include <stdio.h>

#include "board.h"

Move get_user_move();

int main(void) {
  Board board = {};
  init_board(&board);

  while (true) {
    print_board(&board);
    printf("Turn: %s\n", board.white_turn ? "WHITE" : "BLACK");

    auto m = get_user_move();

    if (!execute_move(&board, m)) {
      printf("\033[31mWrong move! Please try again\033[0m\n");
      continue;
    }
  }

  return 0;
}

Move get_user_move() {
  char col_from, col_to;
  int row_from, row_to;

  printf("Enter your move: ");

  while (true) {
    int assigned = scanf(" %c%d %c%d", &col_from, &row_from, &col_to, &row_to);
    if (assigned == 4) {
      break;
    }
    printf("\033[31mWrong move! Please try again\033[0m\n");

    while (getchar() != '\n')
      ;
  }

  return (Move){.from = {.x = toupper(col_from) - 'A', .y = row_from - 1},
                .to = {.x = toupper(col_to) - 'A', .y = row_to - 1}};
}
