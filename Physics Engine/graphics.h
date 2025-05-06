#pragma once
#include "free_body.h"
#include "vector_math.h"
#include "constants.h"
#include <string>
#include <vector>
#include <limits>
#include <SFML/Graphics.hpp>

class Grid : public sf::Drawable
{
public:
	Grid(float width, float height, float spacing, sf::Vector2f max_distance, float y_start, float center_line_width);
	void setColor(sf::Color color);
	void setCenterlineColor(sf::Color color) { center_line.setFillColor(color); };
	void updateGridLines(sf::View& screen, sf::Vector2f center, sf::Vector2f world_size);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(lines, states);
		target.draw(center_line, states);
	}
	sf::VertexArray lines;

	float width, height;
	float spacing;
	float y_start;
	sf::Vector2f max_distance;
	sf::Color color;
	sf::RectangleShape center_line; // Bold center

	// Pointers
	int left_line;
	int right_line;
	int top_line;
	int bottom_line;

	int num_lines_x;
	int num_lines_y;
};

class Box : public sf::Drawable 
{
public:
	sf::Vector2f pos;
	int width, height, line_width;
	Box(sf::Vector2f pos, int height, int width, int line_width);
	void setColor(sf::Color color);
	
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override 
	{
		target.draw(points, states);
	}
	sf::VertexArray points;
};

class Line : public sf::Drawable, public sf::Transformable
{
public:
	sf::Vector2f pos;
	float length;
	int line_width;
	sf::Angle angle;

	Line(sf::Vector2f pos, float length, int line_width, bool is_rounded = false, sf::Angle angle = sf::Angle::Zero);
	void setOrginToCenter();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		// Apply the transform (from sf::Transformable)
		states.transform *= getTransform();
		target.draw(points, states);
	}
	sf::VertexArray points;
	bool is_rounded;
	sf::Vector2f orgin;
};

std::string round_to_string(double num, int decimal_places);

sf::CircleShape getArrow(float inital_size, sf::Color);
