#include "engine.h"

Box createBox()
{
	sf::Vector2f box_pos(Config::BOX_BUFFER, Config::BOX_BUFFER);
	constexpr int BOX_WIDTH = Config::SCREEN_WIDTH - 2 * Config::BOX_BUFFER;
	constexpr int BOX_HEIGHT = Config::SCREEN_HEIGHT - 2 * Config::BOX_BUFFER;

	// A box to enclose objects in the simulation
	Box box(box_pos, BOX_WIDTH, BOX_HEIGHT, Config::BOX_LINE_WIDTH);
	box.setColor(Config::WHITE);
	return box;
}

Grid createGrid()
{
	Grid grid(
		Config::WORLD_WIDTH,
		Config::WORLD_HEIGHT,
		Config::METERS_TO_PIXELS,
		Config::MAX_DISTANCE,
		Config::GROUND_HEIGHT + Config::GROUND_WIDTH,
		Config::GRID_CENTERLINE_WIDTH
	);
	grid.setColor(Config::LIGHT_GRAY);
	grid.setCenterlineColor(Config::LIGHTER_GRAY);
	return grid;
}

sf::RectangleShape createGround()
{
	sf::RectangleShape ground({ Config::MAX_DISTANCE.x * 2, Config::GROUND_HEIGHT });
	ground.setPosition(Config::GROUND_POS);
	ground.setFillColor(Config::GRAY);
	ground.setOutlineColor(Config::WHITE);
	ground.setOutlineThickness(Config::GROUND_WIDTH);
	return ground;
}

void updateView(sf::View& screen)
{
	float viewX{};
	float viewY{};
	// Move view with arrow keys
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) viewX += 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  viewX -= 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    viewY -= 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && screen.getCenter().y < Config::BOTTOM_CENTER.y) 
		viewY += 5;

	screen.move({ viewX, viewY }); // View centered around (viewX, viewY)
}

std::vector<FreeBody> generate_random_free_bodies(long num_free_bodies, Box& box, const FreeBodyConfig& config)
{
	static constexpr double PI{ 3.14159 };
	std::random_device rd; 
	std::mt19937 gen(rd()); // seed the generator

	sf::Vector2f top_left_corner{ box.pos + sf::Vector2f(box.line_width, box.line_width) };
	std::uniform_real_distribution<> dist_real(0.0, 1.0);

	double inner_width = static_cast<double>(box.width - 2 * box.line_width);
	double inner_height = static_cast<double>(box.height - 2 * box.line_width);

	// Float converter
	auto randf = [&]() {
		return static_cast<double>(dist_real(gen));
	};

	std::vector<FreeBody> free_bodies;
	for (long i = 0; i < num_free_bodies; i++) {

		double speed{ static_cast<double>(randf()) * config.max_velocity };
		double angle{ static_cast<double>(randf()) * 2 * PI };

		free_bodies.emplace_back(
			dist_real(gen) * (config.max_mass - config.min_mass) + config.min_mass, // Mass
			top_left_corner + sf::Vector2f(randf() * inner_width, randf() * inner_height), // Initial Position
			sf::Vector2f(speed * std::cos(angle), speed * std::sin(angle)), // Initial Velocity
			config.restitution // Restitution
		);
		free_bodies[i].setColor(Config::ACCENT_COLOR);
	}
	
	return free_bodies;
}

sf::Text createText(const sf::Font& font, sf::Vector2f pos, int size)
{
	sf::Text text(font);
	text.setPosition(pos);
	text.setCharacterSize(size);
	return text;
}

void updateFreeBodies(std::vector<FreeBody>& free_bodies, Box& box, double& accumulator, double& elasped_time)
{
	while (accumulator >= Config::DELTA_TIME)
	{
		for (FreeBody& ball : free_bodies)
		{
			ball.applyForce(ball.mass * Config::GRAVITY);
			ball.update(Config::DELTA_TIME);
			handleBoxCollison(box, ball);
		}
		handleFreeBodyCollisions(free_bodies);
		accumulator -= Config::DELTA_TIME;
	}
}

double getTotalEnergy(const std::vector<FreeBody>& free_bodies, const Box& box)
{
	double total_energy{ 0.0f };
	for (const FreeBody& free_body : free_bodies) {
		total_energy += free_body.getKinticEnergy() 
						+ free_body.getGravitionalEnergy(box.pos.y + box.line_width + box.height);
	}
	return total_energy;
}

double getKineticEnergy(const std::vector<FreeBody>& free_bodies)
{
	double kinetic_energy{ 0.0f };
	for (const FreeBody& free_body : free_bodies) {
		kinetic_energy += free_body.getKinticEnergy();
	}
	return kinetic_energy;
}

double updatedEnergyDerivative(double new_energy_derivative)
{
	static long count = 0;
	static double average = 0.0;

	average = (average * count + new_energy_derivative) / (count + 1);
	count++;

	return average;
}
