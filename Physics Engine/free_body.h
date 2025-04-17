#pragma once
#include <SFML/Graphics.hpp>
#include "vector_math.h"
#include "constants.h"

class FreeBody : public sf::Drawable {
public:
	sf::Vector2f position, velocity, acceleration;
	float mass, restitution;

	FreeBody(float mass, sf::Vector2f inital_pos, sf::Vector2f inital_vel = sf::Vector2f(0, 0), float restitution = 1);
	void setColor(sf::Color color) { circle.setFillColor(color); };
	float getRadius() const { return circle.getRadius(); };

	// Updates position, velocity, and acceleration based on the laws of motion
	void update(float delta_time);
	void applyForce(sf::Vector2f force) { acceleration += force / mass; };

	double getKinticEnergy() const { return (0.5 * mass * squared_magnitude(velocity) / 10'000); };
	double getGravitionalEnergy(float floor_pos) const {
		return (mass * Constants::GRAVITIONAL_FIELD_STRENGTH * ((floor_pos - position.y) / 100));
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(circle, states);
	}

	sf::CircleShape circle;
};