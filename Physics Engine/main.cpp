#include <SFML/Graphics.hpp>
#include "graphics.h"
#include "config.h"
#include "free_body.h"
#include "collison.h"
#include "events.h"
#include "engine.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT }), "SFML works!"
    );
    window.setVerticalSyncEnabled(true);
    sf::Font font(Config::FONT_FILE);

    sf::RectangleShape ground(sf::Vector2f(Config::SCREEN_WIDTH, Config::GROUND_HEIGHT * Config::SCREEN_HEIGHT));
    ground.setPosition(Config::GROUND_POS);
    ground.setFillColor(Config::GRAY);
    ground.setOutlineColor(Config::WHITE);
    ground.setOutlineThickness(Config::GROUND_WIDTH);

    Grid grid(
        Config::SCREEN_WIDTH, 
        Config::SCREEN_HEIGHT,
        100.0f, 
        (Config::SCREEN_HEIGHT - Config::GROUND_POS.y) + Config::GROUND_WIDTH
    );
    grid.setColor(Config::LIGHT_GRAY);

    // Timing variables
    sf::Clock clock;
    double accumulator{ 0.0f };
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Update based on discrete time step in seconds
        double elaspedTime{ static_cast<double>(clock.restart().asSeconds()) };
        accumulator += elaspedTime;

        window.clear(Config::BG_COLOR);
        window.draw(grid);
        window.draw(ground);
        window.display();
    }
}