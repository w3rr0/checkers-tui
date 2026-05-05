#include "board.h"

#include <stdio.h>

void init_board(Board *board) {
  board->white_turn = true;
  for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
      board->grid[y][x] = EMPTY;
      if ((x + y) % 2 != 0) {
        if (y < 3) {
          board->grid[y][x] = BLACK;
        } else if (y > 4) {
          board->grid[y][x] = WHITE;
        }
      }
    }
  }
}

void print_board(const Board *board) {
  printf("\n   ");
  for (int i = 0; i < BOARD_SIZE; i++) {
    printf(" %c ", 'A' + i);
  }
  printf("\n");

  for (int y = 0; y < BOARD_SIZE; y++) {
    printf(" %d ", y + 1);
    for (int x = 0; x < BOARD_SIZE; x++) {
      if ((x + y) % 2 == 0) { // black square
        printf("\033[48;5;250m");
      } else { // white square
        printf("\033[48;5;235m");
      }

      Piece piece = board->grid[y][x];
      switch (piece) {
      case WHITE:
        printf("\033[38;5;15m ●");
        break;
      case BLACK:
        printf("\033[38;5;160m ●");
        break;
      default:
        printf("   ");
        break;
      }
      printf("\033[0m");
    }
    printf("\n");
  }
}
