#ifndef PIECES_H
#define PIECES_H

#include "constant.h"
#include <array>

namespace Piece
{

// 7 pieces
// 4x4 matix
const std::array<PieceMatrix, 7> pieces = {
    // clang-format off
    // O                                              
    0, 0, 0, 0,
    0, 1, 1, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
    // L
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
    // J
    0, 0, 1, 0,
    0, 0, 1, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
    // T
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,
    // S
    0, 0, 0, 0,
    0, 1, 1, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
    // Z
    0, 0, 0, 0,
    0, 1, 1, 0,
    0, 0, 1, 1,
    0, 0, 0, 0,
    // I
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,
    // clang-format on
};

void rotate(PieceMatrix &piece, RotateDirection direction);

void get_random(PieceMatrixColor &output);

} // namespace Piece
#endif // !PIECES_H
