#pragma once
#include <vector>
#include "free_body.h"
#include "graphics.h"
#include "config.h"
#include "collison.h"
#include <random>

// Creates a box on screen with specified width, height, position and line_width
Box createBox();
// Generates a specified number of free bodies with random positions and velocities
std::vector<FreeBody> generate_random_free_bodies(long num_free_bodies, Box& box, const FreeBodyConfig& config);

sf::Text createText(const sf::Font& font, sf::Vector2f pos, int size);

// Updates all free bodies according to the law of physics
void updateFreeBodies(std::vector<FreeBody>& free_bodies, Box& box, double& accumulator, double& elasped_time);

double getTotalEnergy(const std::vector<FreeBody>& free_bodies, const Box& box);
double getKineticEnergy(const std::vector<FreeBody>& free_bodies);

double updatedEnergyDerivative(double new_energy_derivative);

