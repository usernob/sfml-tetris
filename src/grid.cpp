#include "grid.h"
#include "constant.h"
#include "pieces.h"
#include <SFML/System/Vector2.hpp>

// Initialize the grid with empty cells
// also set the vertex position and color
void Grid::_initialize(const sf::Vector2f &pixel_pos, const sf::Vector2f &size)
{
    m_pixelpos = pixel_pos;
    m_size = size;

    m_grid.resize(size.x * size.y);

    // define 6 vertices per cell to draw 2 triangles
    m_vertices.resize(size.x * size.y * VERTICES_PER_CELL);
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    for (int cols = 0; cols < size.x; cols++)
    {
        for (int rows = 0; rows < size.y; rows++)
        {
            m_grid[cols + rows * size.x] = Cell{CellState::EMPTY, CELL_COLOR_DEFAULT};

            // get pointer to first vertex of the current cell
            sf::Vertex *const triangles = &m_vertices[(cols + rows * size.x) * VERTICES_PER_CELL];

            const float cols_pos_0 = (cols * CELLS_SIZE) + m_pixelpos.x;
            const float cols_pos_1 = ((cols + 1) * CELLS_SIZE) + m_pixelpos.x;
            const float rows_pos_0 = (rows * CELLS_SIZE) + m_pixelpos.y;
            const float rows_pos_1 = ((rows + 1) * CELLS_SIZE) + m_pixelpos.y;
            const float padding = 1.5f;

            // set the 6 vertices of the current cell.
            // the cell is made of 2 triangles
            // and has structure like this
            // 0--1   4
            // | /  / |
            // 2   3--5
            //
            // top left
            triangles[0].position = sf::Vector2f(cols_pos_0 + padding, rows_pos_0 + padding);
            // top right
            triangles[1].position = sf::Vector2f(cols_pos_1 - padding, rows_pos_0 + padding);
            // bottom left
            triangles[2].position = sf::Vector2f(cols_pos_0 + padding, rows_pos_1 - padding);
            // bottom left
            triangles[3].position = sf::Vector2f(cols_pos_0 + padding, rows_pos_1 - padding);
            // top right
            triangles[4].position = sf::Vector2f(cols_pos_1 - padding, rows_pos_0 + padding);
            // bottom right
            triangles[5].position = sf::Vector2f(cols_pos_1 - padding, rows_pos_1 - padding);

            // set the default color of the current cell
            _set_color(triangles, CELL_COLOR_DEFAULT);
        }
    }
}

void Grid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_vertices, states);
}

void Grid::_set_color(sf::Vertex *const triangles, const sf::Color &color)
{
    triangles[0].color = color;
    triangles[1].color = color;
    triangles[2].color = color;
    triangles[3].color = color;
    triangles[4].color = color;
    triangles[5].color = color;
}

void Grid::_set_color(const int index, const sf::Color &color)
{
    sf::Vertex *const triangles = &m_vertices[index * VERTICES_PER_CELL];
    _set_color(triangles, color);
}

void GameBoard::update(const sf::Vector2i &new_pos)
{
    // set the previous position to empty
    for (int x = 0; x < PIECE_COLS; x++)
    {
        for (int y = 0; y < PIECE_ROWS; y++)
        {

            const int prev_grid_x = (x + m_current_piece_pos.x);
            const int prev_grid_y = (y + m_current_piece_pos.y);

            if (prev_grid_y >= 0 && prev_grid_y < BOARD_ROWS && prev_grid_x >= 0 &&
                prev_grid_x < BOARD_COLS)
            {
                Cell grid_cell = m_grid[prev_grid_x + prev_grid_y * BOARD_COLS];
                if (grid_cell.state == CellState::ACTIVE)
                {
                    grid_cell.state = CellState::EMPTY;
                    grid_cell.color = CELL_COLOR_DEFAULT;
                    _set_color(prev_grid_x + prev_grid_y * BOARD_COLS, grid_cell.color);
                }
            }
        }
    }

    // set the new position
    for (int x = 0; x < PIECE_COLS; x++)
    {
        for (int y = 0; y < PIECE_ROWS; y++)
        {
            const bool piece_cell = m_current_piece->piece[x + y * PIECE_COLS];
            const int grid_x = (x + new_pos.x);
            const int grid_y = (y + new_pos.y);

            // only update the cell inside the grid
            if (piece_cell && grid_y >= 0 && grid_y < BOARD_ROWS && grid_x >= 0 &&
                grid_x < BOARD_COLS)
            {
                m_grid[grid_x + grid_y * BOARD_COLS] = {
                    .state = CellState::ACTIVE, .color = m_current_piece->color
                };

                _set_color(
                    grid_x + grid_y * BOARD_COLS, m_grid[grid_x + grid_y * BOARD_COLS].color
                );
            }
        }
    }
    m_current_piece_pos = new_pos;
}

bool GameBoard::_check_if_can_move(const PieceMatrix &piece, const sf::Vector2i &new_pos) const
{
    for (int x = 0; x < PIECE_COLS; x++)
    {
        for (int y = 0; y < PIECE_ROWS; y++)
        {
            const bool piece_cell = piece[x + y * PIECE_COLS];
            if (piece_cell)
            {
                const int grid_x = (x + new_pos.x);
                const int grid_y = (y + new_pos.y);
                // if cell is outside the board
                // but cell below the board is still valid
                if (grid_y >= BOARD_ROWS || grid_x < 0 || grid_x >= BOARD_COLS)
                {
                    return false;
                }
                if (grid_y < 0)
                {
                    continue;
                }
                // if cell is already filled
                if (m_grid[grid_x + grid_y * BOARD_COLS].state == CellState::FILLED)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// just for syntactic sugar
bool GameBoard::_check_if_can_rotate(const PieceMatrix &new_rotated_piece) const
{
    return _check_if_can_move(new_rotated_piece, m_current_piece_pos);
}

void GameBoard::_store_piece()
{
    for (int x = 0; x < PIECE_COLS; x++)
    {
        for (int y = 0; y < PIECE_ROWS; y++)
        {
            const bool piece_cell = m_current_piece->piece[x + y * PIECE_COLS];
            if (piece_cell)
            {
                const int grid_x = (x + m_current_piece_pos.x);
                const int grid_y = (y + m_current_piece_pos.y);

                if (piece_cell && grid_y >= 0 && grid_y < BOARD_ROWS && grid_x >= 0 &&
                    grid_x < BOARD_COLS)
                {
                    m_grid[grid_x + grid_y * BOARD_COLS] = {
                        .state = CellState::FILLED, .color = m_current_piece->color
                    };
                    _set_color(
                        grid_x + grid_y * BOARD_COLS, m_grid[grid_x + grid_y * BOARD_COLS].color
                    );
                }
            }
        }
    }
}

void GameBoard::reset_piece()
{
    _store_piece();
    m_current_piece_pos = {3, -2};
}

bool GameBoard::move_piece_down()
{
    const sf::Vector2i new_pos = {m_current_piece_pos.x, m_current_piece_pos.y + 1};
    if (_check_if_can_move(m_current_piece->piece, new_pos))
    {
        update(new_pos);
        return true;
    }
    return false;
}

void GameBoard::move_piece_left()
{
    const sf::Vector2i new_pos = {m_current_piece_pos.x - 1, m_current_piece_pos.y};
    if (_check_if_can_move(m_current_piece->piece, new_pos))
    {
        update(new_pos);
    }
}

void GameBoard::move_piece_right()
{
    const sf::Vector2i new_pos = {m_current_piece_pos.x + 1, m_current_piece_pos.y};
    if (_check_if_can_move(m_current_piece->piece, new_pos))
    {
        update(new_pos);
    }
}

void GameBoard::rotate_piece(const RotateDirection direction)
{
    PieceMatrix rotated_piece;
    for (int i = 0; i < m_current_piece->piece.size(); i++)
    {
        rotated_piece[i] = m_current_piece->piece[i];
    }

    Piece::rotate(rotated_piece, RotateDirection::CLOCKWISE);

    if (_check_if_can_rotate(rotated_piece))
    {
        // if we can rotate, copy the new rotated piece to current piece and redraw
        for (int i = 0; i < rotated_piece.size(); i++)
        {
            m_current_piece->piece[i] = rotated_piece[i];
        }

        update(m_current_piece_pos);
    }
    else
    {
        // else we check if we can do some 'wall kick'
        sf::Vector2i new_pos = {m_current_piece_pos.x, m_current_piece_pos.y};
        bool can_wall_kick = false;

        if (_check_if_can_move(rotated_piece, {m_current_piece_pos.x - 1, m_current_piece_pos.y}))
        {
            new_pos = {m_current_piece_pos.x - 1, m_current_piece_pos.y};
            can_wall_kick = true;
        }
        else if (_check_if_can_move(
                     rotated_piece, {m_current_piece_pos.x + 1, m_current_piece_pos.y}
                 ))
        {
            new_pos = {m_current_piece_pos.x + 1, m_current_piece_pos.y};
            can_wall_kick = true;
        }
        else if (_check_if_can_move(
                     rotated_piece, {m_current_piece_pos.x, m_current_piece_pos.y - 1}
                 ))
        {
            new_pos = {m_current_piece_pos.x, m_current_piece_pos.y - 1};
            can_wall_kick = true;
        }
        else if (_check_if_can_move(
                     rotated_piece, {m_current_piece_pos.x, m_current_piece_pos.y + 1}
                 ))
        {
            new_pos = {m_current_piece_pos.x, m_current_piece_pos.y + 1};
            can_wall_kick = true;
        }

        if (can_wall_kick)
        {
            for (int i = 0; i < rotated_piece.size(); i++)
            {
                m_current_piece->piece[i] = rotated_piece[i];
            }

            update(new_pos);
        }
    }
}

bool GameBoard::is_row_full(const int row) const
{
    int counter = 0;
    for (int i = 0; i < BOARD_COLS; i++)
    {
        if (m_grid[i + row * BOARD_COLS].state == CellState::FILLED)
        {
            counter += 1;
        }
    }
    return counter == BOARD_COLS;
}

bool GameBoard::is_row_empty(const int row) const
{
    int counter = 0;
    for (int i = 0; i < BOARD_COLS; i++)
    {
        if (m_grid[i + row * BOARD_COLS].state == CellState::EMPTY ||
            m_grid[i + row * BOARD_COLS].state == CellState::ACTIVE)
        {
            counter += 1;
        }
    }
    return counter == BOARD_COLS;
}

int GameBoard::get_completed_lines()
{
    int counter = 0;
    int y = BOARD_ROWS - 1;
    while (y >= 0)
    {
        if (is_row_full(y))
        {
            // remove the row
            for (int x = 0; x < BOARD_COLS; x++)
            {
                m_grid[x + y * BOARD_COLS] = {
                    .state = CellState::EMPTY, .color = CELL_COLOR_DEFAULT
                };
                _set_color(x + y * BOARD_COLS, CELL_COLOR_DEFAULT);
            }

            // move down the rows above
            for (int curr_row = y - 1; curr_row >= 0; curr_row--)
            {
                for (int x = 0; x < BOARD_COLS; x++)
                {
                    if (m_grid[x + curr_row * BOARD_COLS].state == CellState::FILLED)
                    {
                        m_grid[x + (curr_row + 1) * BOARD_COLS] = {
                            .state = CellState::FILLED,
                            .color = m_grid[x + curr_row * BOARD_COLS].color
                        };
                        _set_color(
                            x + (curr_row + 1) * BOARD_COLS,
                            m_grid[x + (curr_row + 1) * BOARD_COLS].color
                        );

                        m_grid[x + curr_row * BOARD_COLS] = {
                            .state = CellState::EMPTY, .color = CELL_COLOR_DEFAULT
                        };
                        _set_color(x + curr_row * BOARD_COLS, CELL_COLOR_DEFAULT);
                    }
                }
            }
            counter++;
        }
        else
        {
            y--;
        }
    }
    return counter;
}

void NextPieceBoard::update_piece(const PieceMatrixColor &piece_matrix)
{
    for (int i = 0; i < piece_matrix.piece.size(); i++)
    {
        m_grid[i].state = piece_matrix.piece[i] ? CellState::FILLED : CellState::EMPTY;
        m_grid[i].color = piece_matrix.piece[i] ? piece_matrix.color : CELL_COLOR_DEFAULT;
        _set_color(i, m_grid[i].color);
    }
}
