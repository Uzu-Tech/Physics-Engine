#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>

namespace Config {
	// Grpahics
	inline constexpr int SCREEN_HEIGHT{ 1800 };
	inline constexpr int SCREEN_WIDTH{ 3200 };
	inline constexpr int VIEWPORT_SCALE_X{ 3 };
	inline constexpr int VIEWPORT_SCALE_Y{ 2 };

	// Grid
	inline constexpr float METERS_TO_PIXELS{ 100.0f };
	inline constexpr sf::Vector2f GRID_MAX_KM( 100.0f, 20.0f ); // Past 20km the gravitional field strength stops being constant
	inline constexpr float GRID_CENTERLINE_WIDTH{ 10.0f };

	// The space between the edge of the window and the sides of the box
	inline constexpr int BOX_BUFFER{ 200 };
	inline constexpr int BOX_LINE_WIDTH{ 30 };

	// Ground Settings
	inline constexpr float GROUND_HEIGHT_SCALE{ 0.16f };
	inline constexpr int GROUND_WIDTH{ 10 };

	// Fixed time step between update calls
	inline constexpr double DELTA_TIME{ 1e-4f };

	inline constexpr sf::Color BG_COLOR(34, 40, 49);
	inline constexpr sf::Color WHITE(238, 238, 238);
	inline constexpr sf::Color ACCENT_COLOR(249, 237, 105);
	inline constexpr sf::Color GRAY(57, 62, 70);
	inline constexpr sf::Color LIGHT_GRAY(96, 104, 117);
	inline constexpr sf::Color LIGHTER_GRAY(148, 156, 168);

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

	// Calculated Values
	inline constexpr int WORLD_HEIGHT{ SCREEN_HEIGHT * VIEWPORT_SCALE_Y };
	inline constexpr int WORLD_WIDTH{ SCREEN_WIDTH * VIEWPORT_SCALE_X };
	inline constexpr sf::Vector2f WORLD_CENTER(static_cast<float>(WORLD_WIDTH) / 2, static_cast<float>(WORLD_HEIGHT) / 2);
	inline constexpr sf::Vector2f BOTTOM_CENTER(WORLD_CENTER.x, WORLD_HEIGHT - static_cast<float>(SCREEN_HEIGHT) / 2);

	inline constexpr sf::Vector2f MAX_DISTANCE(
		GRID_MAX_KM.x * METERS_TO_PIXELS * 1'000, GRID_MAX_KM.y * METERS_TO_PIXELS * 1'000
	);

	inline constexpr float GROUND_HEIGHT{ GROUND_HEIGHT_SCALE * SCREEN_HEIGHT };
	inline constexpr sf::Vector2f GROUND_POS(-MAX_DISTANCE.x, static_cast<float>(WORLD_HEIGHT) - GROUND_HEIGHT);
	inline constexpr float ABOVE_GROUND_HEIGHT{ static_cast<float>(WORLD_HEIGHT) - GROUND_HEIGHT };

}

// Structs for function arguments

// Arguments used to generate a random free body
struct FreeBodyConfig {
	float max_velocity{ 1200.0f };
	float min_mass{ 30.0f };
	float max_mass{ 50.0f };
	float restitution{ 1.0f };
};