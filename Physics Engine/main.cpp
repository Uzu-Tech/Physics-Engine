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

    // A box that encloses the balls used for collisons
    Box box{ createBox() };

    FreeBodyConfig free_body_config;
    // Balls that experince the physical forces & can collide with other objects
    std::vector<FreeBody> balls{ generate_random_free_bodies(Config::NUM_FREE_BODIES, box, free_body_config) };


    sf::Font font(Config::FONT_FILE);
    sf::Text energy_text{ createText(font, Config::TEXT_POS, Config::TEXT_SIZE) };

    double last_energy_value{};
    double energy_derivative{};

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
           
        updateFreeBodies(balls, box, accumulator, elaspedTime);

        window.clear(Config::BG_COLOR);
        window.draw(box);

        /// TODO put in seperate function and calculate long-run energy loss
        double total_energy{ getTotalEnergy(balls, box) };
        
        if (last_energy_value) {
            double new_energy_derivative = (total_energy - last_energy_value) / elaspedTime;
            energy_derivative = updatedEnergyDerivative(new_energy_derivative);
        }
        last_energy_value = total_energy;

        for (const FreeBody& ball : balls) 
        {
            window.draw(ball);
        }

        energy_text.setString(Config::KE_TEXT + std::to_string(energy_derivative));
        window.draw(energy_text);
        window.display();
    }
}