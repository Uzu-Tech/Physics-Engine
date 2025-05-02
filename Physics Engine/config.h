#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>

namespace Config {
	// Grpahics

	inline constexpr int SCREEN_HEIGHT{ 1800 };
	inline constexpr int SCREEN_WIDTH{ 3200 };
	inline constexpr sf::Vector2f CENTER(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	// The space between the edge of the window and the sides of the box
	inline constexpr int BOX_BUFFER{ 200 };
	inline constexpr int BOX_LINE_WIDTH{ 30 };

	// Ground Settings
	inline constexpr float GROUND_HEIGHT{ 0.16 };
	inline constexpr sf::Vector2f GROUND_POS(0.0f, static_cast<float>(SCREEN_HEIGHT) * (1 - GROUND_HEIGHT));
	inline constexpr int GROUND_WIDTH{ 10 };

	// Fixed time step between update calls
	inline constexpr double DELTA_TIME{ 1e-4f };

	inline constexpr sf::Color BG_COLOR(34, 40, 49);
	inline constexpr sf::Color WHITE(238, 238, 238);
	inline constexpr sf::Color ACCENT_COLOR(249, 237, 105);
	inline constexpr sf::Color GRAY(57, 62, 70);
	inline constexpr sf::Color LIGHT_GRAY(96, 104, 117);

	// Free Body Properties

	inline constexpr long NUM_FREE_BODIES = 20;

	inline constexpr sf::Vector2f GRAVITY(0.0f, 981.0f);

	// Text Properties
	const std::filesystem::path FONT_FILE{ "Poppins-Regular.ttf" };
	inline constexpr sf::Vector2f TEXT_POS(700, 100);
	inline constexpr int TEXT_SIZE{ 50 };
	inline constexpr int TEXT_DECIMAL_PLACES{ 3 };
	// Text displayed before showing the energy value e.g. "Total Energy: "
	const std::string KE_TEXT{ "Energy Loss J/s: " };
}

// Structs for function arguments

// Arguments used to generate a random free body
struct FreeBodyConfig {
	float max_velocity{ 1200.0f };
	float min_mass{ 30.0f };
	float max_mass{ 50.0f };
	float restitution{ 1.0f };
};