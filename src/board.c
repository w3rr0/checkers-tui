#include "board.h"
#include "pawn.h"
#include "theme.h"

#include <stdio.h>
#include <stdlib.h>

void init_board(Board *board) {
  board->white_turn = true;
  board->white_pieces = STARTING_PIECES;
  board->black_pieces = STARTING_PIECES;
  board->start_time = time(nullptr);
  board->total_moves = 0;
  board->disconnected = false;
  board->game_over = false;
  board->timeout = false;
  for (int y = 0; y < BOARD_SIZE; ++y) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
      board->grid[y][x] = PIECE_EMPTY;
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
  for (int i = 0; i < BOARD_SIZE; ++i) {
    printf(" %c ", 'A' + i);
  }
  printf("\n");

  for (int y = 0; y < BOARD_SIZE; ++y) {
    printf(" %d ", y + 1);
    for (int x = 0; x < BOARD_SIZE; ++x) {
      if ((x + y) % 2 == 0) { // black square
        printf("%s", black_square);
      } else { // white square
        printf("%s", white_square);
      }

      char *color_code;
      char *piece_symbol;

      auto piece = board->grid[y][x];

      switch (piece.color) {
      case WHITE:
        color_code = white_color_code;
        break;
      case BLACK:
        color_code = black_color_code;
        break;
      default:
        color_code = "";
        break;
      }

      switch (piece.type) {
      case PAWN:
        piece_symbol = pawn_symbol;
        break;
      case QUEEN:
        piece_symbol = queen_symbol;
        break;
      default:
        piece_symbol = empty_symbol;
        break;
      }

      printf("%s%s", color_code, piece_symbol);
      printf("%s", reset_color);
    }
    printf("\n");
  }
}

[[nodiscard]] bool _is_on_board(Coordinate cord) {
  return cord.x >= 0 && cord.x < BOARD_SIZE && cord.y >= 0 &&
         cord.y < BOARD_SIZE;
}

void _update_counter_after_capture(Board *board) {
  if (board->white_turn) {
    board->black_pieces--;
  } else {
    board->white_pieces--;
  }
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
  if (actor.type == PAWN && dx != 1 &&
      dx != 2) { // 1 -> regular move, 2 -> capture pawn
    return false;
  }

  // direction of movement
  if (actor.type == PAWN) {

    if (actor.color == WHITE && move.from.y >= move.to.y) {
      return false;
    }
    if (actor.color == BLACK && move.from.y <= move.to.y) {
      return false;
    }
  }

  if (actor.type == PAWN && dx == 2) {
    int mid_x = (move.from.x + move.to.x) / 2;
    int mid_y = (move.from.y + move.to.y) / 2;
    auto mid_piece = board->grid[mid_y][mid_x];

    if (mid_piece.type == EMPTY_TYP || mid_piece.color == actor.color) {
      return false;
    }

    // capturing successful
    board->grid[mid_y][mid_x] = PIECE_EMPTY;
    _update_counter_after_capture(board);
  }

  if (actor.type == QUEEN) {
    Coordinate current = {.x = move.from.x, .y = move.from.y};
    auto piece_seen = false;
    Coordinate piece_cord;

    int step_x = (move.to.x > move.from.x) - (move.to.x < move.from.x);
    int step_y = (move.to.y > move.from.y) - (move.to.y < move.from.y);

    while (!match_coordinates(current, move.to)) {
      // Next field
      current.x += step_x;
      current.y += step_y;

      auto type = board->grid[current.y][current.x].type;
      if (type != EMPTY_TYP) {
        if (!piece_seen) {
          piece_seen = true;
          piece_cord = current;
        } else {
          return false;
        }
      }
    }

    // Capture piece
    if (piece_seen) {
      board->grid[piece_cord.y][piece_cord.x] = PIECE_EMPTY;
      _update_counter_after_capture(board);
    }
  }

  // Execute move
  board->grid[move.to.y][move.to.x] = actor;
  board->grid[move.from.y][move.from.x] = PIECE_EMPTY;
  board->total_moves++;

  // check for queen trannformation
  if (actor.type == PAWN) {
    if (actor.color == WHITE && move.to.y == BOARD_SIZE - 1) {
      make_queen(&board->grid[move.to.y][move.to.x]);
    } else if (actor.color == BLACK && move.to.y == 0) {
      make_queen(&board->grid[move.to.y][move.to.x]);
    }
  }

  // Change turn
  board->white_turn = !board->white_turn;

  return true;
}
