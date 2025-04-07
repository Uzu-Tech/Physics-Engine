#include "collison.h"

void handleBoxCollison(Box& box, FreeBody& free_body)
{
	float collison_ceiling = box.pos.y + box.line_width + free_body.getRadius();
	float collison_floor = box.pos.y - box.line_width + box.height - free_body.getRadius();
	float collison_left = box.pos.x + box.line_width + free_body.getRadius();
	float collison_right = box.pos.x - box.line_width + box.width - free_body.getRadius();

	if (free_body.position.y <= collison_ceiling || free_body.position.y >= collison_floor) {
		free_body.velocity.y = -free_body.velocity.y * free_body.restitution;
		free_body.position.y = (free_body.position.y <= collison_ceiling) ? collison_ceiling : collison_floor;
	}
	
	if (free_body.position.x <= collison_left || free_body.position.x >= collison_right) {
		free_body.velocity.x = -free_body.velocity.x * free_body.restitution;
		free_body.position.x = (free_body.position.x <= collison_left) ? collison_ceiling : collison_right;
	}
}
