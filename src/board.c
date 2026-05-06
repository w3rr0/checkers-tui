#include "board.h"
#include "pawn.h"

#include <stdio.h>
#include <stdlib.h>

void init_board(Board *board) {
  board->white_turn = true;
  for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
      board->grid[y][x] = (Piece){.color = EMPTY_COL, .type = EMPTY_TYP};
      if ((x + y) % 2 != 0) {
        if (y < 3) {
          board->grid[y][x] = (Piece){.color = WHITE, .type = PAWN};
        } else if (y > 4) {
          board->grid[y][x] = (Piece){.color = BLACK, .type = PAWN};
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

      auto piece_col = board->grid[y][x].color;
      switch (piece_col) {
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

  auto actor = board->grid[move.from.y][move.from.x];
  auto target = board->grid[move.to.y][move.to.x];

  // Check pawn from
  if (actor.type == EMPTY_TYP) {
    return false;
  }
  if (board->white_turn && actor.color != WHITE) {
    return false;
  }
  if (!board->white_turn && actor.color != BLACK) {
    return false;
  }

  // Check target field
  if (target.type != EMPTY_TYP) {
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
  if (actor.color == WHITE && move.from.y >= move.to.y) {
    return false;
  }
  if (actor.color == BLACK && move.from.y <= move.to.y) {
    return false;
  }

  if (dx == 2) {
    int mid_x = (move.from.x + move.to.x) / 2;
    int mid_y = (move.from.y + move.to.y) / 2;
    auto mid_piece = board->grid[mid_y][mid_x];

    if (mid_piece.type == EMPTY_TYP || mid_piece.color == actor.color) {
      return false;
    }

    // capturing successful
    board->grid[mid_y][mid_x] = (Piece){.color = EMPTY_COL, .type = EMPTY_TYP};
  }

  // Execute move
  board->grid[move.to.y][move.to.x] = actor;
  board->grid[move.from.y][move.from.x] =
      (Piece){.color = EMPTY_COL, .type = EMPTY_TYP};

  // Change turn
  board->white_turn = !board->white_turn;

  return true;
}
