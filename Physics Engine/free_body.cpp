#include "free_body.h"
#include <algorithm>

FreeBody::FreeBody(float mass, sf::Vector2f inital_pos, sf::Vector2f inital_vel, float restitution)
	: mass(mass), position(inital_pos), velocity(inital_vel), restitution(restitution)
{
	circle = sf::CircleShape(mass, static_cast<std::size_t>(std::max(100.0f, mass)));
	circle.setOrigin(sf::Vector2f(mass, mass));
	circle.setPosition(position);
}

void FreeBody::update(float delta_time)
{
	// Euler method
	velocity = velocity + delta_time * acceleration;
	position = position + delta_time * velocity;
	circle.setPosition(position);
	acceleration = sf::Vector2f(0, 0);
}