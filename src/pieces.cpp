#include "pieces.h"
#include "constant.h"
#include "utils.h"

namespace Piece
{

void get_random(PieceMatrixColor &ouput)
{
    const int index = Utils::random_int(0, 6);
    // copy the matrix to output
    for (int i = 0; i < Piece::pieces[index].size(); i++)
    {
        ouput.piece[i] = Piece::pieces[index][i];
    }
    ouput.color = Utils::random_color();
}

// REF: https://stackoverflow.com/questions/42519/how-do-you-rotate-a-two-dimensional-array
void rotate(PieceMatrix &ouput, RotateDirection direction)
{
    const int layer_count = PIECE_COLS / 2;

    for (int layer = 0; layer < layer_count; layer++)
    {
        const int first = layer;
        const int last = PIECE_COLS - first - 1;

        for (int element = first; element < last; element++)
        {
            const int offset = element - first;

            const int top_index = first * PIECE_COLS + element;
            const int right_side_index = element * PIECE_COLS + last;
            const int bottom_index = last * PIECE_COLS + (last - offset);
            const int left_side_index = (last - offset) * PIECE_COLS + first;

            const bool top = ouput[top_index];
            const bool right_side = ouput[right_side_index];
            const bool bottom = ouput[bottom_index];
            const bool left_side = ouput[left_side_index];

            if (direction == RotateDirection::CLOCKWISE)
            {
                ouput[top_index] = left_side;
                ouput[right_side_index] = top;
                ouput[bottom_index] = right_side;
                ouput[left_side_index] = bottom;
            }
            else
            {
                ouput[top_index] = right_side;
                ouput[right_side_index] = bottom;
                ouput[bottom_index] = left_side;
                ouput[left_side_index] = top;
            }
        }
    }
}

} // namespace Piece
