#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>

namespace Config {
	// Grpahics

	constexpr int SCREEN_HEIGHT{ 2000 };
	constexpr int SCREEN_WIDTH{ 2000 };
	constexpr sf::Vector2f CENTER(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	// The space between the edge of the window and the sides of the box
	constexpr int BOX_BUFFER{ 200 };
	constexpr int BOX_LINE_WIDTH{ 30 };

	// Fixed time step between update calls
	constexpr double DELTA_TIME{ 1e-2f };

	constexpr sf::Color BG_COLOR(34, 40, 49);
	constexpr sf::Color WHITE(238, 238, 238);
	constexpr sf::Color ACCENT_COLOR(249, 237, 105);
	constexpr sf::Color GRAY(57, 62, 70);

	// Free Body Properties

	constexpr long NUM_FREE_BODIES = 150;

	constexpr sf::Vector2f GRAVITY(0.0f, 981.0f);

	// Text Properties

	const std::filesystem::path FONT_FILE{ "Poppins-Regular.ttf" };
	constexpr sf::Vector2f TEXT_POS(700, 100);
	constexpr int TEXT_SIZE{ 50 };
	constexpr int TEXT_DECIMAL_PLACES{ 3 };
	// Text displayed before showing the energy value e.g. "Total Energy: "
	const std::string KE_TEXT{ "Energy Loss J/s: " };
}

// Structs for function arguments

// Arguments used to generate a random free body
struct FreeBodyConfig {
	float max_velocity{ 1200.0f };
	float min_mass{ 5.0f };
	float max_mass{ 5.0f };
	float restitution{ 1.0f };
};