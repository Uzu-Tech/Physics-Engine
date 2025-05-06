#include "graphics.h"

// Grid
Grid::Grid(float width, float height, float spacing, sf::Vector2f max_distance, float y_start, float center_line_width)
	: width(width), 
	  height(height), 
	  spacing(spacing),
	  max_distance(max_distance),
	  y_start(y_start),
	  lines(sf::PrimitiveType::Lines),
	  center_line{}, left_line{}, right_line{}, top_line{}, bottom_line{},
	  num_lines_x{}, num_lines_y{}
{
	float center_x{ width / 2 };
	
	for (float offset = -std::floor(center_x / spacing); offset < std::floor(center_x / spacing); offset += 1)
	{
		float pos_x{ center_x + offset * spacing };
		lines.append(sf::Vertex{ {pos_x, (height - y_start)} });
		lines.append(sf::Vertex{ {pos_x, (height - y_start) - max_distance.y} });
		num_lines_x++;
	}
	right_line = num_lines_x - 1;

	for (float offset = -std::floor(y_start / spacing); offset <= std::floor((height - y_start) / spacing); offset += 1)
	{
		float pos_y{ (height - y_start) - offset * spacing};
		lines.append(sf::Vertex{ {-max_distance.x + center_x, pos_y} });
		lines.append(sf::Vertex{ {max_distance.x + center_x, pos_y} });
		num_lines_y++;
	}
	top_line = num_lines_y - 1;

	center_line.setSize(sf::Vector2f(center_line_width, max_distance.y));
	center_line.setOrigin(sf::Vector2f(center_line_width / 2, 0.0f));
	center_line.setPosition(sf::Vector2f(center_x, (height - y_start) - max_distance.y));
}

void Grid::setColor(sf::Color color)
{
	for (size_t i = 0; i < lines.getVertexCount(); i++) 
	{ 
		lines[i].color = color;
		center_line.setFillColor(color);
	}
}

void Grid::updateGridLines(sf::View& screen, sf::Vector2f center, sf::Vector2f world_size)
{
	enum Direction { LEFT, RIGHT, TOP, BOTTOM };
	sf::Vector2f offset = screen.getCenter() - center;

	auto getIndex = [&](int idx, Direction dir) -> int {
		switch (dir) {
		case Direction::LEFT:
		case Direction::RIGHT:
			return idx * 2;
		case Direction::BOTTOM:
		default:
			return (idx + num_lines_x) * 2;
		}
		};

	// Moves a line’s vertices horizontally by a given amount
	auto shiftLineHorizontally = [&](int idx, Direction dir) {
		int shift{ (dir == LEFT)? 1 : -1 };
		lines[idx].position.x += shift * world_size.x;
		lines[idx + 1].position.x += shift * world_size.x;
		left_line = (left_line + shift + num_lines_x) % num_lines_x;
		right_line = (right_line + shift + num_lines_x) % num_lines_x;
		};

	// Moves a line’s vertices vertically by a given amount
	auto shiftLineVertically = [&](int idx, Direction dir) {
		int shift{ (dir == TOP) ? 1 : -1 };
		lines[idx].position.y += shift * world_size.y;
		lines[idx + 1].position.y += shift * world_size.y;
		bottom_line = (bottom_line - shift + num_lines_y) % num_lines_y;
		top_line = (top_line - shift + num_lines_y) % num_lines_y;
		};

	// LEFT → move lines forward until they're right of the screen center
	int left_idx{ getIndex(left_line, LEFT) };
	while (lines[left_idx].position.x < offset.x) {
		shiftLineHorizontally(left_idx, LEFT);
		left_idx = getIndex(left_line, LEFT);
	}

	// RIGHT → move lines backward until they're left of the screen center
	int right_idx = getIndex(right_line, RIGHT);
	while (lines[right_idx].position.x > (offset.x + world_size.x)) {
		shiftLineHorizontally(right_idx, RIGHT);
		right_idx = getIndex(right_line, RIGHT);
	}

	// RIGHT → move lines backward until they're left of the screen center
	int top_idx = getIndex(top_line, TOP);
	while (lines[top_idx].position.y < offset.y) {
		shiftLineVertically(top_idx, TOP);
		top_idx = getIndex(top_line, TOP);
	}

	// LEFT → move lines forward until they're right of the screen center
	int bottom_idx{ getIndex(bottom_line, BOTTOM) };
	while (lines[bottom_idx].position.y > (offset.y + world_size.y)) {
		shiftLineVertically(bottom_idx, BOTTOM);
		bottom_idx = getIndex(bottom_line, BOTTOM);
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
