#include <SFML/Graphics.hpp>
#include "graphics.h"
#include "config.h"
#include "free_body.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT }), "SFML works!"
    );

    sf::Vector2f box_pos(Config::BOX_BUFFER, Config::BOX_BUFFER);
    constexpr int BOX_WIDTH = Config::SCREEN_WIDTH - 2 * Config::BOX_BUFFER;
    constexpr int BOX_HEIGHT = Config::SCREEN_HEIGHT - 2 * Config::BOX_BUFFER;

    Box box(box_pos, BOX_WIDTH, BOX_HEIGHT, Config::BOX_LINE_WIDTH);
    box.setColor(Config::WHITE);

    FreeBody ball(Config::TEST_MASS, Config::CENTER);

    sf::Clock clock;
    sf::Vector2f GRAVITY(0.0f, 981.0f);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        ball.applyForce(ball.mass * GRAVITY);
        float deltaTime = clock.restart().asSeconds();
        ball.update(deltaTime);

        window.clear(Config::BG_COLOR);
        window.draw(box);
        window.draw(ball);
        window.display();
    }
}