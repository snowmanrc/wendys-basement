
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "include/game.hpp"
#include "include/map.hpp"
#include "include/player.hpp"


namespace Game {

    void init(State& state) {

        state.running = true;

        state.window = sf::RenderWindow(sf::VideoMode({720, 600}), "Wendy's Basement");
        state.window.setFramerateLimit(120);
        state.camera.setSize({1920, 1080});

        state.player = {
            .shape = sf::CircleShape(16),
            .position = {200, 200},
            .size = 16,
            .speed = Player::walking_speed
        };
        
        Map::load("");
    }


    void handleEvents(State& state) {
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

    void update(State& state) {
        state.dt = state.clock.restart().asSeconds();

        Player::updatePosition(state.player, state.dt);
    }


    void render(State& state) {
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
                shape.setFillColor(sf::Color::Yellow);
                shape.setPosition({x, y});
                state.window.draw(shape);
            }
        }
        state.player.shape.setPosition(state.player.position);
        state.player.shape.setFillColor(sf::Color::Red);
        state.window.draw(state.player.shape);

        state.window.display();
    }


    void end(State& state) {
        
        if (!state.running) return;

        if (state.window.isOpen()) {
            state.window.close();
        }
    }


    void run() {

        State state;
        init(state);

        while (state.running) {
            handleEvents(state);
            update(state);
            render(state);
        }

        end(state);
    }

}