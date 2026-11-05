#include "../include/definitions.hpp"
#include "../include/mechanics.hpp"
#include "../include/core.hpp"

#include <SFML/Graphics/RectangleShape.hpp>


void initGame(GameState& state) {
    state.window = sf::RenderWindow(sf::VideoMode({720, 600}), "Wendy's Basement");
	state.window.setFramerateLimit(120);
	state.is_running = true;

	state.player.shape = sf::CircleShape(10);
	state.player.position = {200, 200};
	state.player.speed = 500;
	state.player.shape.setPosition(state.player.position);
	
	state.camera.setSize({1920, 1080});

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {

			state.map.grid[i][j] = std::rand() % 5 == 0 ? 1: 0;
		}
	}
}


void handleEvents(GameState& state) {
    while (const auto event = state.window.pollEvent()) {

		if (event->is<sf::Event::Closed>()) {
			state.is_running = false;
		}

		if (const auto key = event->getIf<sf::Event::KeyPressed>()) {
			if (key->code == sf::Keyboard::Key::Escape) {
				state.is_running = false;
			}
		}
	}
}


void updateGame(GameState& state) {

    /*
    dt here means 'delta time'
    it calculates the seconds between two frames
    we use this so that calculations are done with respect to seconds and not with respect to frames
    */

    state.dt = state.clock.restart().asSeconds();

	updatePlayerPosition(state);	
}


void renderGame(GameState& state) {
    state.window.clear();
	state.window.setView(state.camera);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {

			auto tile = state.map.grid[i][j];
			
			if (tile == 0) continue;

			float x = j * TILE_SIZE;
			float y = i * TILE_SIZE;

			auto shape = sf::RectangleShape({TILE_SIZE, TILE_SIZE});
			shape.setPosition({x, y});
			state.window.draw(shape);
		}
	}
    state.player.shape.setPosition(state.player.position);
	state.player.shape.setFillColor(sf::Color::Red);
	state.window.draw(state.player.shape);
	state.window.display();
}

void runGame() {
    GameState state;
	initGame(state);	

	while (state.is_running) {
		handleEvents(state);
		updateGame(state);
		renderGame(state);
	}

	if (state.window.isOpen()) {
		state.window.close();
	}
}