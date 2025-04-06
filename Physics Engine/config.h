#pragma once
#include <SFML/Graphics.hpp>

namespace Config {
	// Grpahics
	constexpr int SCREEN_HEIGHT = 2000;
	constexpr int SCREEN_WIDTH = 2000;
	constexpr sf::Vector2f CENTER(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	constexpr int BOX_BUFFER = 200;
	constexpr int BOX_LINE_WIDTH = 15;

	constexpr sf::Color BG_COLOR(34, 40, 49);
	constexpr sf::Color WHITE(238, 238, 238);
	constexpr sf::Color ACCENT_COLOR(249, 237, 105);

	constexpr float TEST_MASS = 50.0f;
}