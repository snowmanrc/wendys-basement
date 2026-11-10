
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "include/game.hpp"
#include "include/debug.hpp"
#include "include/ghost.hpp"
#include "include/map.hpp"
#include "include/player.hpp"
#include "include/graphics.hpp"
#include "include/utils.hpp"


namespace Game {

    void init(State& state) {
        state.running = true;

        state.window = sf::RenderWindow(sf::VideoMode({720, 600}), "Wendy's Basement");
        state.window.setFramerateLimit(120);
        state.camera.setSize({1920, 1080});

        state.player = Player::Entity({1200, 880});
        state.ghost = Ghost::Entity({10, 40});

        Map::load("assets/map.png");

        DEBUG("GAME: Initialized State");
    }


    void handleEvents(State& state) {
        while (const auto event = state.window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                Game::end(state);
            }

            if (const auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    Game::end(state);
                }
            }
        }
    }

    void update(State& state) {

        state.dt = state.clock.restart().asSeconds();

        Player::updatePosition(state.player, state.dt);       
        Ghost::updatePosition(state.ghost, state.dt);


        static sf::Clock pathfindingClock;
        sf::Time pathfindingInterval = sf::seconds(0.1f); // Run every 0.5 seconds

        // Inside your main game loop:
        sf::Time elapsed = pathfindingClock.getElapsedTime();

        if (elapsed >= pathfindingInterval) {
            sf::Vector2i player_tile = Utils::cartesianToTiledVector(state.player.position, Map::tilesize);
            Ghost::follow(state.ghost, player_tile); 
            // Reset the clock for the next interval
            pathfindingClock.restart();
        }

        if (Ghost::hasCaughtPlayer(state.ghost, state.player)) state.running = false;
    }


    void render(State& state) {

        state.window.clear();
        state.camera.setCenter(state.player.position);
        state.window.setView(state.camera);
        
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

        DEBUG("GAME: Bye Bye");
    }


    void run() {

        State state;

        while (true) {

            init(state);

            int chosen = Graphics::triggerMenu(state.window, "Wendy's Basement", "Play", "Quit");
            if (chosen == 2) break; 
            
            while (state.running) {
                handleEvents(state);
                update(state);
                render(state);
            }
        }
        
        end(state);
    }

}