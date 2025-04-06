#include "free_body.h"
#include <algorithm>

FreeBody::FreeBody(float mass, sf::Vector2f inital_pos, sf::Vector2f inital_vel, float restitution, sf::Color color)
	: mass(mass), position(inital_pos), velocity(inital_vel), restitution(restitution)
{
	shape = sf::CircleShape(mass, static_cast<std::size_t>(std::max(100.0f, mass)));
	shape.setOrigin(sf::Vector2f(mass, mass));
	shape.setPosition(position);
	shape.setFillColor(color);
}

void FreeBody::update(float delta_time)
{
	velocity = velocity + delta_time * acceleration;
	position = position + delta_time * velocity;
	shape.setPosition(position);
	acceleration = sf::Vector2f(0, 0);
}

void FreeBody::applyForce(sf::Vector2f force)
{
	acceleration += force / mass;
}
