#ifndef GAME_H
#define GAME_H

#include "constant.h"
#include "grid.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

class Game
{
private:
    unsigned int score;
    bool game_over;
    GameBoard board;
    NextPieceBoard next_piece_board;


    sf::Font font;
    sf::Text text_score, next_piece_text;
    sf::Music bg_music;
    sf::SoundBuffer piece_rotate_soundbuf, clear_row_soundbuf, game_over_soundbuf;
    sf::Sound piece_rotate_sound, clear_row_sound, game_over_sound;

    PieceMatrixColor current_piece;
    PieceMatrixColor next_piece;
    sf::Clock down_clock;


public:
    Game();

    Game(const Game &) = delete;
    Game(Game &&) = delete;
    Game &operator=(const Game &) = delete;
    Game &operator=(Game &&) = delete;

    static const sf::Vector2u get_size()
    {
        return {
            static_cast<unsigned int>((BOARD_COLS * CELLS_SIZE) * 1.8),
            (BOARD_ROWS * CELLS_SIZE) + 20
        };
    };

    void reset();
    void load();
    void initilize();

    void handle_input();

    void update();

    void draw(sf::RenderWindow &window) const;
};
#endif // !GAME_H
