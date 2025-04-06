#include "graphics.h"

Box::Box(sf::Vector2f pos, int width, int height, int line_width)
	: points(sf::PrimitiveType::TriangleStrip, 10)
{
	// Outer box
	points[0].position = pos;
	points[2].position = sf::Vector2f(pos.x + width, pos.y);
	points[4].position = sf::Vector2f(pos.x + width, pos.y + height);
	points[6].position = sf::Vector2f(pos.x, pos.y + height);
	points[8].position = pos;

	int inner_width = width - 2 * line_width;
	int inner_height = height - 2 * line_width;
	sf::Vector2f inner_pos(pos.x + line_width, pos.y + line_width);

	// Inner box
	points[1].position = inner_pos;
	points[3].position = sf::Vector2f(inner_pos.x + inner_width, inner_pos.y);
	points[5].position = sf::Vector2f(inner_pos.x + inner_width, inner_pos.y + inner_height);
	points[7].position = sf::Vector2f(inner_pos.x, inner_pos.y + inner_height);
	points[9].position = inner_pos;
}

void Box::setColor(sf::Color color)
{
	for (std::size_t i = 0; i < points.getVertexCount(); ++i) {
		points[i].color = color;
	}
}
