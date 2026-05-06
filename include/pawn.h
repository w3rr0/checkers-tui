#ifndef PAWN_H
#define PAWN_H

#include <stdint.h>

typedef enum : uint8_t { EMPTY_COL, WHITE, BLACK } PieceColor;
typedef enum : uint8_t { EMPTY_TYP, PAWN, QUEEN } PieceType;

typedef struct {
  PieceColor color;
  PieceType type;
} Piece;

bool make_queen(Piece *piece);

#endif // PAWN_H
