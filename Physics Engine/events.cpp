#include "events.h"

bool isFreeBodyPressed(const sf::Event::MouseButtonPressed* mouseButtonPressed, const FreeBody& free_body)
{
	sf::Vector2f mousePosFloat = static_cast<sf::Vector2f>(mouseButtonPressed->position);
	return magnitude(mousePosFloat - free_body.position) <= free_body.getRadius();
}

void handleMouseDown(
    sf::CircleShape& arrow,
    FreeBody& free_body,
    const sf::Vector2i& mouse_pos,
    const ArrowConfig& config // Pass the config struct
)
{
    // Convert mouse position to float
    sf::Vector2f mouse_pos_float = static_cast<sf::Vector2f>(mouse_pos);

    // Calculate direction and normal vectors
    sf::Vector2f direction_vector = free_body.position - mouse_pos_float;
    sf::Vector2f normal_vector = direction_vector / magnitude(direction_vector);

    // Calculate the strength based on the distance
    float strength;
    if (magnitude(direction_vector) <= free_body.getRadius()) {
        strength = 0;
    }
    else
    {
        float mouse_multipler = (
            magnitude(direction_vector) - free_body.getRadius()) / (config.max_distance - free_body.getRadius()
                );
        strength = std::min(config.min_arrow_strength + mouse_multipler, 1.0f);
    }

    // Adjust arrow size and position based on strength
    float arrow_radius = strength * free_body.getRadius() * config.arrow_size_multiplier;
    // PLaces the triangle away from the circle equal to buffer
    float arrow_distance = 1.5f * free_body.getRadius() + config.buffer;

    arrow.setRadius(arrow_radius);
    arrow.setOrigin(sf::Vector2f(arrow_radius, arrow_radius));

    // Set rotation to match the direction vector
    arrow.setRotation(getAngle(direction_vector) + sf::radians(1.570796327f)); // 90 degrees
    // Set arrow position based on normal vector
    arrow.setPosition(normal_vector * arrow_distance + free_body.position);
    // Set the velocity of the free body based on the calculated strength and max_velocity
    free_body.velocity = normal_vector * strength * config.max_velocity;
}