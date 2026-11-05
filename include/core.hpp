#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "definitions.hpp"

// Main entry point of the game. Contains the main game loop and call to other core functions
void runGame();

// Intializes and starts up the game resources and state
void initGame(GameState& state);

// Handles events that the window receives
void handleEvents(GameState& state);

// Updates the game state depending upon current state and conditions
void updateGame(GameState& state);

// Triggers the drawing of the game
void renderGame(GameState& state);