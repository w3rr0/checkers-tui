#include "pawn.h"

bool make_queen(Piece *piece) {
  if (piece->type == QUEEN) {
    return false;
  }
  piece->type = QUEEN;
  return true;
}
