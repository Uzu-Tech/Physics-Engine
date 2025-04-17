#pragma once
#include "graphics.h"
#include "free_body.h"
#include <cmath>
#include <SFML/Graphics.hpp>

void handleBoxCollison(Box& box, FreeBody& free_body);
void handleFreeBodyCollisions(std::vector<FreeBody>& free_bodies);
void handleFreeBodyCollision(FreeBody& free_body1, FreeBody& free_body2);