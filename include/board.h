#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>

constexpr uint8_t BOARD_SIZE = 8;

typedef enum : uint8_t { EMPTY = 0, WHITE, BLACK } Piece;

typedef struct {
  Piece grid[BOARD_SIZE][BOARD_SIZE];
  bool white_turn;
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
bool execute_move(Board *board, Move move);

#endif // BOARD_H
