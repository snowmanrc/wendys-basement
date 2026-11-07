#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

static const int MAP_WIDTH = 100;
static const int MAP_HEIGHT = 100;
static const float TILE_SIZE = 40;

static const float PLAYER_RADIUS = 12;

struct Map {
    int grid[MAP_WIDTH][MAP_HEIGHT];
};

struct Player {
    sf::CircleShape shape;
    sf::Vector2f position;
	float size;
    float speed;
};


struct GameState {
    sf::RenderWindow window;
	sf::View camera;
	sf::Clock clock;
	float dt;
	bool is_running;
	Map map;
	Player player;
};