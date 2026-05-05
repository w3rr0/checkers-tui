#include <stdio.h>

#include "board.h"

int main(void) {
  Board board = {};
  init_board(&board);
  print_board(&board);

  printf("Turn: %s\n", board.white_turn ? "WHITE" : "BLACK");
  return 0;
}
