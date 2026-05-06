#include "board.h"

#include <stdio.h>
#include <stdlib.h>

void init_board(Board *board) {
  board->white_turn = true;
  for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
      board->grid[y][x] = EMPTY;
      if ((x + y) % 2 != 0) {
        if (y < 3) {
          board->grid[y][x] = WHITE;
        } else if (y > 4) {
          board->grid[y][x] = BLACK;
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
        printf("\033[38;5;15m ● ");
        break;
      case BLACK:
        printf("\033[38;5;160m ● ");
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

[[nodiscard]] bool _is_on_board(Coordinate cord) {
  return cord.x >= 0 && cord.x < BOARD_SIZE && cord.y >= 0 &&
         cord.y < BOARD_SIZE;
}

bool execute_move(Board *board, Move move) {
  // Check voordinates
  if (!_is_on_board(move.from) || !_is_on_board(move.to)) {
    return false;
  }

  Piece actor = board->grid[move.from.y][move.from.x];
  Piece target = board->grid[move.to.y][move.to.x];

  // Check pawn from
  if (actor == EMPTY) {
    return false;
  }
  if (board->white_turn && actor != WHITE) {
    return false;
  }
  if (!board->white_turn && actor != BLACK) {
    return false;
  }

  // Check target field
  if (target != EMPTY) {
    return false;
  }

  // Check diagonal movement
  int dx = abs(move.from.x - move.to.x);
  int dy = abs(move.from.y - move.to.y);
  if (dx != dy) {
    return false;
  }
  if (dx != 1 && dx != 2) { // 1 -> regular move, 2 -> capture pawn
    return false;
  }

  // direction of movement
  if (actor == WHITE && move.from.y >= move.to.y) {
    return false;
  }
  if (actor == BLACK && move.from.y <= move.to.y) {
    return false;
  }

  if (dx == 2) {
    int mid_x = (move.from.x + move.to.x) / 2;
    int mid_y = (move.from.y + move.to.y) / 2;
    Piece mid_piece = board->grid[mid_y][mid_x];

    if (mid_piece == EMPTY || mid_piece == actor) {
      return false;
    }

    // capturing successful
    board->grid[mid_y][mid_x] = EMPTY;
  }

  // Execute move
  board->grid[move.to.y][move.to.x] = actor;
  board->grid[move.from.y][move.from.x] = EMPTY;

  // Change turn
  board->white_turn = !board->white_turn;

  return true;
}
