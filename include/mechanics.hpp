#pragma once

#include "definitions.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

void updatePlayerPosition(GameState& state);

bool hasPlayerCollided(GameState& state);