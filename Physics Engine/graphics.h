#pragma once
#include "free_body.h"
#include "vector_math.h"
#include <string>
#include <SFML/Graphics.hpp>

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

std::string round_to_string(double num, int decimal_places);

sf::CircleShape getArrow(float inital_size, sf::Color);
