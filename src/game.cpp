
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "game.hpp"
#include "map.hpp"
#include "player.hpp"


void Game::init(Game::State& state) {

    state.running = true;

    state.window = sf::RenderWindow(sf::VideoMode({720, 600}), "Wendy's Basement");
	state.window.setFramerateLimit(60);

    state.player = {
        .shape = sf::CircleShape(16),
        .position = {200, 200},
        .size = 16,
        .speed = Player::walking_speed
    };
    
    Map::load("");
}


void Game::handleEvents(Game::State& state) {
    while (const auto event = state.window.pollEvent()) {

		if (event->is<sf::Event::Closed>()) {
			state.running = false;
		}

		if (const auto key = event->getIf<sf::Event::KeyPressed>()) {
			if (key->code == sf::Keyboard::Key::Escape) {
				state.running = false;
			}
		}
	}
}

void Game::update(Game::State& state) {
    state.dt = state.clock.restart().asSeconds();

    Player::updatePosition(state.player, state.dt);
}


void Game::render(Game::State& state) {
    state.window.clear();
	state.window.setView(state.camera);
    state.camera.setCenter(state.player.position);

	for (int i = 0; i < Map::height; i++) {
		for (int j = 0; j < Map::width; j++) {

			auto tile = Map::grid[i][j];
			
			if (tile == 0) continue;

			float x = j * Map::tilesize;
			float y = i * Map::tilesize;

			auto shape = sf::RectangleShape({Map::tilesize, Map::tilesize});
			shape.setPosition({x, y});
			state.window.draw(shape);
		}
	}
	state.player.shape.setPosition(state.player.position);
	state.player.shape.setFillColor(sf::Color::Red);
	state.window.draw(state.player.shape);

	state.window.display();
}


void Game::end(Game::State& state) {
    
    if (!state.running) return;

    if (state.window.isOpen()) {
        state.window.close();
    }
}


void Game::run() {

    Game::State state;
    Game::init(state);

    while (state.running) {
        Game::handleEvents(state);
        Game::update(state);
        Game::render(state);
    }

    Game::end(state);
}