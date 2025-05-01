#pragma once
#include "free_body.h"
#include "vector_math.h"
#include "constants.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Grid : public sf::Drawable
{
public:
	Grid(float width, float height, float spacing, float y_start = 0.0f);
	void setColor(sf::Color color);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(lines, states);
		target.draw(centerLine, states);
	}
	sf::VertexArray lines;

	float width, height;
	float spacing;
	float y_start;
	sf::Color color;
	sf::RectangleShape centerLine; // Bold center
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
