#ifndef GRID_H
#define GRID_H

#include "constant.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

class Grid : public sf::Drawable
{
protected:
    using GridArray = std::vector<Cell>;
    GridArray m_grid;

    sf::VertexArray m_vertices;

    sf::Vector2f m_pixelpos, m_size;
    void _set_color(sf::Vertex *const triangles, const sf::Color &color);
    void _set_color(const int index, const sf::Color &color);
    void _initialize(const sf::Vector2f &pixel_pos, const sf::Vector2f &size);

public:
    Grid() {};

    Grid(const Grid &) = delete;
    Grid(Grid &&) = delete;
    Grid &operator=(const Grid &) = delete;
    Grid &operator=(Grid &&) = delete;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f get_pixelpos() const
    {
        return m_pixelpos;
    }

    unsigned int get_width() const
    {
        return m_size.x * CELLS_SIZE;
    }

    unsigned int get_height() const
    {
        return m_size.y * CELLS_SIZE;
    }

    sf::VertexArray &get_vertices()
    {
        return m_vertices;
    }

    std::vector<Cell> &get_grid()
    {
        return m_grid;
    }
};

class GameBoard : public Grid
{
private:
    PieceMatrixColor *m_current_piece;
    sf::Vector2i m_current_piece_pos;

public:
    GameBoard() : m_current_piece_pos({3, -2}) {}

    GameBoard(const GameBoard &) = delete;
    GameBoard(GameBoard &&) = delete;
    GameBoard &operator=(const GameBoard &) = delete;
    GameBoard &operator=(GameBoard &&) = delete;

    void initialize(const sf::Vector2f &pos, PieceMatrixColor *current_piece)
    {
        _initialize(pos, {BOARD_COLS, BOARD_ROWS});
        m_current_piece = current_piece;
    }

    void reset_piece();

    bool move_piece_down();

    void move_piece_left();

    void move_piece_right();

    void rotate_piece(const RotateDirection direction);

    int get_completed_lines();

    void update(const sf::Vector2i &new_pos);

    bool is_row_full(const int row) const;
    bool is_row_empty(const int row) const;

private:
    bool _check_if_can_move(const PieceMatrix &piece, const sf::Vector2i &new_pos) const;

    bool _check_if_can_rotate(const PieceMatrix &new_rotated_piece) const;


    void _store_piece();
};

class NextPieceBoard : public Grid
{
public:
    NextPieceBoard() {}

    NextPieceBoard(const NextPieceBoard &) = delete;
    NextPieceBoard(NextPieceBoard &&) = delete;
    NextPieceBoard &operator=(const NextPieceBoard &) = delete;
    NextPieceBoard &operator=(NextPieceBoard &&) = delete;

    void initialize(const sf::Vector2f &pos)
    {
        _initialize(pos, {PIECE_COLS, PIECE_ROWS});
    }

    void update_piece(const PieceMatrixColor &piece_matrix);
};

#endif // !GRID_H
