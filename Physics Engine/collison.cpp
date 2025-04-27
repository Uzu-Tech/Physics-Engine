#include "collison.h"

void handleBoxCollison(Box& box, FreeBody& free_body)
{
	// Get the positions of the floor, ceiling and sides of the box from the box coordinates
	float collison_ceiling{ box.pos.y + box.line_width + free_body.getRadius() };
	float collison_floor{ box.pos.y - box.line_width + box.height - free_body.getRadius() };
	float collison_left{ box.pos.x + box.line_width + free_body.getRadius() };
	float collison_right{ box.pos.x - box.line_width + box.width - free_body.getRadius() };

	// Resolve ceiling or floor collison
	if (free_body.position.y <= collison_ceiling || free_body.position.y >= collison_floor) {
		free_body.velocity.y = -free_body.velocity.y * std::sqrt(free_body.restitution);
		free_body.position.y = (free_body.position.y <= collison_ceiling) ? collison_ceiling : collison_floor;
	}
	
	// Resolve side collison
	if (free_body.position.x <= collison_left || free_body.position.x >= collison_right) {
		free_body.velocity.x = -free_body.velocity.x * std::sqrt(free_body.restitution);
		free_body.position.x = (free_body.position.x <= collison_left) ? collison_left : collison_right;
	}
}

void handleFreeBodyCollisions(std::vector<FreeBody>& free_bodies)
{
	for (size_t i = 0; i < free_bodies.size(); i++) {
		for (size_t j = i + 1; j < free_bodies.size(); j++) {
			if (
				magnitude(free_bodies[i].position - free_bodies[j].position)
				< free_bodies[i].getRadius() + free_bodies[j].getRadius()
			)
			{
				handleFreeBodyCollision(free_bodies[i], free_bodies[j]);
			}

		}
	}
}

void handleFreeBodyCollision(FreeBody& free_body1, FreeBody& free_body2) {
	// Resolve overlap by shifting the first free body out of free body 2
	float distance{ magnitude(free_body1.position - free_body2.position) };
	float overlap{
		free_body1.getRadius() + free_body2.getRadius() - distance
	};
	sf::Vector2f collision_normal{
		(free_body1.position - free_body2.position) / distance
	};
	free_body1.position += overlap * collision_normal;

	// Calculate new velocities based on conversation of momentum and kinetic energy along axis of collision
	sf::Vector2f collision_tangent(-collision_normal.y, collision_normal.x);

	// u represents the magnitude of intial velocity along the collison axis
	float u1{ dot(free_body1.velocity, collision_normal) };
	float u2{ dot(free_body2.velocity, collision_normal) };

	// Factors used in equation
	float mass_difference_factor{ (free_body1.mass - free_body2.mass) / (free_body1.mass + free_body2.mass) };
	float partner_mass_factor1{ 2 * free_body2.mass / (free_body1.mass + free_body2.mass) };
	float partner_mass_factor2{ 2 * free_body1.mass / (free_body1.mass + free_body2.mass) };

	// v represents the final velocity along the collison axis
	float v1{ mass_difference_factor * u1 + partner_mass_factor1 * u2 };
	float v2{ -mass_difference_factor * u2 + partner_mass_factor2 * u1 };

	sf::Vector2f v1_normal{ v1 * collision_normal };
	sf::Vector2f v2_normal{ v2 * collision_normal };

	sf::Vector2f v1_tangent{ dot(free_body1.velocity, collision_tangent) * collision_tangent };
	sf::Vector2f v2_tangent{ dot(free_body2.velocity, collision_tangent) * collision_tangent };

	free_body1.velocity = v1_normal + v1_tangent;
	free_body2.velocity = v2_normal + v2_tangent;
}

