
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "include/game.hpp"
#include "include/ghost.hpp"
#include "include/map.hpp"
#include "include/player.hpp"
#include "include/graphics.hpp"


namespace Game {

    void init(State& state) {

        state.running = true;

        state.window = sf::RenderWindow(sf::VideoMode({720, 600}), "Wendy's Basement");
        state.window.setFramerateLimit(120);
        state.camera.setSize({1920, 1080});

        state.player = {
            .shape = sf::CircleShape(16),
            .position = {200, 100},
            .size = 16,
            .speed = Player::walking_speed
        };

        state.ghost = {
            .shape = sf::CircleShape(16),
            .position = {10, 400},
            .size = 16,
            .speed = 500,
        };

        Map::load("");
        sf::Vector2i player_tile = {
            static_cast<int>(state.player.position.x / Map::tilesize), 
            static_cast<int>(state.player.position.y / Map::tilesize)
        };
        Ghost::follow(state.ghost, player_tile); 
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
        Ghost::updatePosition(state.ghost, state.dt);


        static sf::Clock pathfindingClock;
        sf::Time pathfindingInterval = sf::seconds(0.5f); // Run every 0.5 seconds

        // Inside your main game loop:
        sf::Time elapsed = pathfindingClock.getElapsedTime();

        if (elapsed >= pathfindingInterval) {
            sf::Vector2i player_tile = {
                static_cast<int>(state.player.position.x / Map::tilesize), 
                static_cast<int>(state.player.position.y / Map::tilesize)
            };
            Ghost::follow(state.ghost, player_tile); 
            // Reset the clock for the next interval
            pathfindingClock.restart();
        }
    }


    void render(State& state) {
        state.window.clear();
        state.window.setView(state.camera);
        state.camera.setCenter(state.player.position);

        Graphics::drawMap(state.window);
        Graphics::drawPlayer(state.window, state.player);
        Graphics::drawGhost(state.window, state.ghost);

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