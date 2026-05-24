#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "pawn.h"

constexpr uint8_t BOARD_SIZE = 8;
constexpr uint8_t STARTING_PIECES = 12;

typedef struct {
  Piece grid[BOARD_SIZE][BOARD_SIZE];
  bool white_turn;
  uint8_t white_pieces;
  uint8_t black_pieces;
  
  time_t start_time;
  uint32_t total_moves;
  bool disconnected;
  bool game_over;
  bool timeout;
} Board;

typedef struct {
  uint8_t x;
  uint8_t y;
} Coordinate;

typedef struct {
  Coordinate from;
  Coordinate to;
} Move;

void init_board(Board *board);
void print_board(const Board *board);
void print_statistics(const Board *board);
bool execute_move(Board *board, Move move);
static inline bool match_coordinates(Coordinate a, Coordinate b) {
  return a.x == b.x && a.y == b.y;
}

#endif // BOARD_H
