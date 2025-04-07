#pragma once
#include <SFML/Graphics.hpp>
#include "config.h"

class FreeBody : public sf::Drawable {
public:
	sf::Vector2f position, velocity, acceleration;
	float mass, restitution;

	FreeBody(
		float mass,
		sf::Vector2f inital_pos,
		sf::Vector2f inital_vel = sf::Vector2f(0, 0),
		float restitution = 1,
		sf::Color color = Config::ACCENT_COLOR
	);
	void update(float delta_time);
	void applyForce(sf::Vector2f force);

	float getRadius() const { return circle.getRadius(); };

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(circle, states);
	}

	sf::CircleShape circle;
};