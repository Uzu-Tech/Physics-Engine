#include "graphics.h"

// Grid
Grid::Grid(float width, float height, float spacing, float y_start)
	: width(width), 
	  height(height), 
	  spacing(spacing), 
	  y_start(y_start),
	  lines(sf::PrimitiveType::Lines),
	  centerLine{}
{
	float center_x{ width / 2 };
	
	for (float offset = -std::floor(center_x / spacing); offset < std::floor(center_x / spacing); offset += 1)
	{
		if (offset == 0) continue;
		float pos_x{ center_x + offset * spacing };
		lines.append(sf::Vertex{ {pos_x, 0} });
		lines.append(sf::Vertex{ {pos_x, height} });
	}

	for (float offset = 1; offset <= std::floor((height - y_start) / spacing); offset += 1)
	{
		float pos_y{ (height - y_start) - offset * spacing};
		lines.append(sf::Vertex{ {0, pos_y} });
		lines.append(sf::Vertex{ {width, pos_y} });
	}

	centerLine.setSize(sf::Vector2f(10.0f, (height - y_start)));
	centerLine.setOrigin(sf::Vector2f(5.0f, 0.f));
	centerLine.setPosition(sf::Vector2f(center_x, 0.0f));
}

void Grid::setColor(sf::Color color)
{
	for (int i = 0; i < lines.getVertexCount(); i++) 
	{ 
		lines[i].color = color;
		centerLine.setFillColor(color);
	}
}

// Box

Box::Box(sf::Vector2f pos, int width, int height, int line_width)
	: pos(pos), height(height), width(width), line_width(line_width), points(sf::PrimitiveType::TriangleStrip, 10)
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

// Line

constexpr int NUM_RECT_VERTICES = 4;
Line::Line(sf::Vector2f pos, float length, int line_width, bool is_rounded, sf::Angle angle)
	: pos(pos), 
	  length(length), 
	  line_width(line_width),
	  is_rounded(is_rounded),
	  points(sf::PrimitiveType::TriangleFan, (is_rounded)? line_width * 2 : NUM_RECT_VERTICES)
{
	// Create first semi circle for start of line
	float radius{ static_cast<float>(line_width) / 2 };

	// Create a rectangle if not rounded
	if (!is_rounded)
	{
		std::vector<sf::Vector2f> dirs = {
			sf::Vector2f(0, radius),
			sf::Vector2f(0, -radius),
			sf::Vector2f(length, -radius),
			sf::Vector2f(length, radius),
		};

		for (int i = 0; i < NUM_RECT_VERTICES; i++)
		{
			points[i].position = dirs[i];
		}
	}
	else
	{
		// Create first semi circle for start of rounded line
		int num_points{ line_width };

		auto createSemiCircle = [&](int starting_idx, int direction, sf::Vector2f center)
			{
				for (int i = 0; i < num_points; i++)
				{
					double angle = static_cast<double>(i) / (num_points - 1) * Constants::PI;
					points[starting_idx + i].position =
						center + radius * sf::Vector2f(
							static_cast<float>(direction * std::sin(angle)),
							static_cast<float>(direction * std::cos(angle))  // only cos here
						);
				}
			};

		createSemiCircle(0, -1, sf::Vector2f(radius, 0)); // -1 direction creates the semi-circle facing left
		createSemiCircle(num_points, 1, sf::Vector2f(length - radius, 0)); // 1 direction creates the semi-circle facing right
	}
	setPosition(pos);
	setRotation(angle);
}

void Line::setOrginToCenter()
{
	setOrigin(sf::Vector2f(length / 2.f, 0.f));
}

std::string round_to_string(double num, int decimal_places)
{
	double factor{ std::pow(10.0f, decimal_places) };
	int scaled_num{ static_cast<int>(std::floor(factor * num)) };
	
	int int_part{ scaled_num / static_cast<int>(factor) };
	int frac_part{ scaled_num % static_cast<int>(factor) };

	return std::to_string(int_part) + "." + std::to_string(frac_part);
}

sf::CircleShape getArrow(float inital_size, sf::Color color)
{
	sf::CircleShape arrow(inital_size, 3);
	arrow.setFillColor(color);
	return arrow;
}
