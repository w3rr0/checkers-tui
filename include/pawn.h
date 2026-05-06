#ifndef PAWN_H
#define PAWN_H

#include <stdint.h>

#define PIECE_EMPTY                                                            \
  (Piece) { .color = EMPTY_COL, .type = EMPTY_TYP }

typedef enum : uint8_t { EMPTY_COL, WHITE, BLACK } PieceColor;
typedef enum : uint8_t { EMPTY_TYP, PAWN, QUEEN } PieceType;

constexpr auto pawn_symbol[] = " ● ";
constexpr auto queen_symbol[] = " ♕ ";
constexpr auto empty_symbol[] = "   ";
constexpr auto white_color_code[] = "\033[38;5;15m";
constexpr auto black_color_code[] = "\033[38;5;160m";
constexpr auto reset_color[] = "\033[0m";

typedef struct {
  PieceColor color;
  PieceType type;
} Piece;

bool make_queen(Piece *piece);

#endif // PAWN_H
