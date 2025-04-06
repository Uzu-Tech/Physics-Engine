#pragma once

#include <SFML/Graphics.hpp>;

class Box : public sf::Drawable {
public:
	Box(sf::Vector2f pos, int height, int width, int line_width);
	void setColor(sf::Color color);
	
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(points, states);
	}
	sf::VertexArray points;
};