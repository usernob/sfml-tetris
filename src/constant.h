#ifndef CONSTANT_H
#define CONSTANT_H

#include <SFML/Graphics/Color.hpp>
#include <array>

#ifndef ASSETS_PATH
#define ASSETS_PATH "./assets"
#endif // !ASSETS_PATH


#define BOARD_ROWS 20
#define BOARD_COLS 10

#define CELLS_SIZE 43
#define VERTICES_PER_CELL 6

#define PIECE_ROWS 4
#define PIECE_COLS 4

using PieceMatrix = std::array<bool, PIECE_ROWS * PIECE_COLS>;

const sf::Color CELL_COLOR_DEFAULT = sf::Color(48, 51, 52);
const sf::Color TEXT_COLOR_DEFAULT = sf::Color(177, 181, 184);

const std::array<sf::Color, 6> Colors = {
    sf::Color(224, 108, 117),
    sf::Color(152, 195, 121),
    sf::Color(97, 175, 239),
    sf::Color(229, 192, 123),
    sf::Color(198, 120, 221),
    sf::Color(86, 182, 194),
};

enum class RotateDirection
{
    CLOCKWISE = 1,
    COUNTER_CLOCKWISE = -1
};

enum class CellState
{
    EMPTY,
    ACTIVE,
    FILLED
};

struct Cell
{
    CellState state;
    sf::Color color;
};

struct PieceMatrixColor
{
    PieceMatrix piece;
    sf::Color color;
};

#endif // !CONSTANT_H
