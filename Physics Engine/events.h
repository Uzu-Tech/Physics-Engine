#pragma once
#include <SFML/Window.hpp>
#include "vector_math.h"
#include "free_body.h"

struct ArrowConfig {
	float buffer = 30.0f;  // Default buffer
	float max_distance = 300.0f;  // Default max mouse distance
	float max_velocity = 1250.0f;  // Default max velocity
	float arrow_size_multiplier = 1.2f;  // Scaling factor for the arrow's size
	float min_arrow_strength = 0.2f;  // Minimum arrow strength
};

bool isFreeBodyPressed(const sf::Event::MouseButtonPressed* mouseButtonPressed, const FreeBody& free_body);
void handleMouseDown(
	sf::CircleShape& arrow,
	FreeBody& free_body,
	const sf::Vector2i& mouse_pos,
	const ArrowConfig& config
);