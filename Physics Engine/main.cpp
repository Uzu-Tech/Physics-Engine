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

    sf::RectangleShape ground{ createGround() };
    constexpr CollisionBoundary ground_boundary{ getGroundBoundary() };
    Grid grid{ createGrid() };

    sf::View screen(
        { Config::BOTTOM_CENTER.x, Config::BOTTOM_CENTER.y }, 
        { Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT }
    );

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

        updateView(screen);
        grid.updateGridLines(screen, Config::BOTTOM_CENTER, { Config::WORLD_WIDTH, Config::WORLD_HEIGHT });

        // Update based on discrete time step in seconds
        double elaspedTime{ static_cast<double>(clock.restart().asSeconds()) };
        accumulator += elaspedTime;

        window.clear(Config::BG_COLOR);
        window.setView(screen);
        window.draw(grid);
        window.draw(ground);
        window.display();
    }
}