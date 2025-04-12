#include "game.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_LOG(str) std::cout << str << "\n";
#else
#define DEBUG_LOG(str)
#endif

int main()
{
    DEBUG_LOG("ASSETS_PATH: " << ASSETS_PATH);
    // create window with fixed size and disable resize
    sf::RenderWindow window = sf::RenderWindow(
        sf::VideoMode(Game::get_size()), "Tetris", sf::Style::Titlebar | sf::Style::Close
    );

    window.setVerticalSyncEnabled(true);

    // set window center
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i{
        static_cast<int>(desktop.size.x * 0.5 - window.getSize().x * 0.5),
        static_cast<int>(desktop.size.y * 0.5 - window.getSize().y * 0.5)
    });


    Game game;

    try
    {
        game.initilize();
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << "\n";
        return -1;
    }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::KeyPressed>())
            {
                game.handle_input();
            }
        }

        window.clear(sf::Color(12, 13, 13));

        game.update();
        game.draw(window);

        window.display();
    }
}
