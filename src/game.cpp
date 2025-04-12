#include "game.h"
#include "constant.h"
#include "pieces.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Game::Game()
    : score(0), game_over(false), text_score(font), next_piece_text(font),
      piece_rotate_sound(piece_rotate_soundbuf), clear_row_sound(clear_row_soundbuf),
      game_over_sound(game_over_soundbuf)
{
}

void Game::initilize()
{
    reset();
    load();
}

void Game::load()
{
    if (!font.openFromFile(ASSETS_PATH "/font/monogram.ttf"))
    {
        throw "Failed to load font\n";
    }
    font.setSmooth(false);

    if (!bg_music.openFromFile(ASSETS_PATH "/sounds/retro-game-music-230227.mp3"))
    {
        throw "Failed to load music\n";
    }

    bg_music.setLooping(true);
    bg_music.play();

    if (!piece_rotate_soundbuf.loadFromFile(ASSETS_PATH "/sounds/sfx_jump_07-80241.mp3") ||
        !clear_row_soundbuf.loadFromFile(ASSETS_PATH
                                         "/sounds/retro-video-game-coin-pickup-38299.mp3") ||
        !game_over_soundbuf.loadFromFile(ASSETS_PATH "/sounds/game-over-arcade-6435.mp3"))
    {
        throw "Failed to load sound\n";
    }

    text_score.setFont(font);
    text_score.setFillColor(TEXT_COLOR_DEFAULT);
    text_score.setCharacterSize(40);
    text_score.setPosition({board.get_pixelpos().x + board.get_width() + 10, 10});
    text_score.setString("Score: " + std::to_string(score));

    next_piece_text.setFont(font);
    next_piece_text.setFillColor(TEXT_COLOR_DEFAULT);
    next_piece_text.setCharacterSize(40);
    next_piece_text.setPosition({board.get_pixelpos().x + board.get_width() + 10, 60});
    next_piece_text.setString("Next piece");
}

void Game::reset()
{
    game_over = false;
    score = 0;
    Piece::get_random(current_piece);
    Piece::get_random(next_piece);

    board.initialize({10, 10}, &current_piece);
    board.update({3, -1});

    next_piece_board.initialize({board.get_pixelpos().x + board.get_width() + 7, 120});

    next_piece_board.update_piece(next_piece);
}

void Game::update()
{
    if (game_over)
    {
        return;
    }
    if (down_clock.getElapsedTime().asMilliseconds() >= 550)
    {
        down_clock.restart();

        // game over if the first row is not empty
        game_over = !board.is_row_empty(0);
        if (game_over)
        {
            bg_music.pause();
            game_over_sound.play();
            return;
        }

        // if the piece can't move down
        // - reset the piece pos
        // - get a new piece
        // - check if there is complete row
        // - update the score
        if (!board.move_piece_down())
        {
            board.reset_piece();

            current_piece = next_piece;
            Piece::get_random(next_piece);
            next_piece_board.update_piece(next_piece);

            int count = board.get_completed_lines();
            if (count > 0)
            {
                score += count * 100 + (std::pow(2, count) - 1) * 50;
                text_score.setString("Score: " + std::to_string(score));
                clear_row_sound.play();
            }
        }
    }
}

void Game::draw(sf::RenderWindow &window) const
{
    if (game_over)
    {
        sf::Text game_over_text(font), final_score_text(font), instruction_text(font);

        game_over_text.setFillColor(TEXT_COLOR_DEFAULT);
        game_over_text.setCharacterSize(60);
        game_over_text.setString("Game Over");
        game_over_text.setOrigin({
            game_over_text.getLocalBounds().size.x * 0.5f,
            game_over_text.getLocalBounds().size.y,
        });
        game_over_text.setPosition({
            static_cast<float>(get_size().x * 0.5),
            static_cast<float>(
                (get_size().y - game_over_text.getLocalBounds().size.y - 40.0f) * 0.5
            ),
        });

        final_score_text.setFillColor(TEXT_COLOR_DEFAULT);
        final_score_text.setCharacterSize(40);
        final_score_text.setString("Final score: " + std::to_string(score));
        final_score_text.setOrigin({
            final_score_text.getLocalBounds().size.x * 0.5f,
            final_score_text.getLocalBounds().size.y * 0.5f,
        });
        final_score_text.setPosition({
            static_cast<float>(get_size().x * 0.5),
            static_cast<float>(get_size().y * 0.5),
        });

        instruction_text.setFillColor(TEXT_COLOR_DEFAULT);
        instruction_text.setCharacterSize(40);
        instruction_text.setString("Press [ENTER] to restart");
        instruction_text.setOrigin({
            instruction_text.getLocalBounds().size.x * 0.5f,
            0,
        });
        instruction_text.setPosition({
            static_cast<float>(get_size().x * 0.5),
            static_cast<float>(
                (get_size().y + instruction_text.getLocalBounds().size.y + 40.0f) * 0.5
            ),
        });

        window.draw(game_over_text);
        window.draw(final_score_text);
        window.draw(instruction_text);
    }
    else
    {
        window.draw(board);
        window.draw(text_score);
        window.draw(next_piece_text);
        window.draw(next_piece_board);
    }
}

void Game::handle_input()
{
    if (game_over)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            reset();
            bg_music.play();
            return;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        board.move_piece_left();
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        board.move_piece_right();
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        piece_rotate_sound.play();
        board.rotate_piece(RotateDirection::CLOCKWISE);
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        board.move_piece_down();
        return;
    }
}
