#pragma once
#include "graphics.h"
#include "free_body.h"
#include <cmath>
#include <SFML/Graphics.hpp>

struct CollisionBoundary
{
	float left{};
	float right{};
	float ceiling{};
	float floor{};
};

void handleRigidCollison(CollisionBoundary& boundary, FreeBody& free_body);

void handleFreeBodyCollisions(std::vector<FreeBody>& free_bodies);
void handleFreeBodyCollision(FreeBody& free_body1, FreeBody& free_body2);