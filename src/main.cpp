#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <iostream>

struct Map { 
	static const int width = 100; 
	static const int height = 100; 
	static constexpr float tile_size = 40.0f; // 0 = Floor, 1 = Wall 
	int grid[width][height];
};

struct Player {
	sf::CircleShape shape;
	sf::Vector2f pos;
	float speed;
};

struct GameState {
	sf::RenderWindow window;
	sf::View view;
	sf::Clock clock;
	float dt;
	bool is_running;
	Map map;
	Player player;
};


void initGame(GameState& state) {
	state.window = sf::RenderWindow(sf::VideoMode({720, 600}), "Wendy's Basement");
	state.window.setFramerateLimit(120);
	state.is_running = true;

	state.player.shape = sf::CircleShape(10);
	state.player.pos = {200, 200};
	state.player.speed = 500;
	state.player.shape.setPosition(state.player.pos);
	
	state.view.setSize({1920, 1080});

	for (int i = 0; i < state.map.height; i++) {
		for (int j = 0; j < state.map.width; j++) {

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

	state.dt = state.clock.restart().asSeconds();

	sf::Vector2f direction{0.0f, 0.0f};

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) direction.y -= 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction.x -= 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) direction.y += 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction.x += 1.0f;

	if (direction.x && direction.y) {
		direction.x /= std::sqrt(2);
		direction.y /= std::sqrt(2);
	}

	state.player.pos += direction * state.player.speed * state.dt;
	state.player.shape.setPosition(state.player.pos);
	state.player.shape.setFillColor(sf::Color::Red);
	state.view.setCenter(state.player.pos);
}

void renderGame(GameState& state) {
	state.window.clear();
	state.window.setView(state.view);

	for (int i = 0; i < state.map.height; i++) {
		for (int j = 0; j < state.map.width; j++) {

			auto tile = state.map.grid[i][j];
			
			if (tile == 0) continue;

			float x = j * state.map.tile_size;
			float y = i * state.map.tile_size;

			auto shape = sf::RectangleShape({state.map.tile_size, state.map.tile_size});
			shape.setPosition({x, y});
			state.window.draw(shape);
		}
	}
	state.window.draw(state.player.shape);
	state.window.display();
}


int main() {
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

	return 0;
}