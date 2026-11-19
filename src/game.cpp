
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

#include "include/game.hpp"
#include "include/debug.hpp"
#include "include/ghost.hpp"
#include "include/map.hpp"
#include "include/player.hpp"
#include "include/graphics.hpp"
#include "include/sound.hpp"
#include "include/utils.hpp"


namespace Game {

    void init(State& state) {

        state.window = sf::RenderWindow(sf::VideoMode({720, 600}), "Wendy's Basement");
        state.window.setFramerateLimit(120);

        state.camera = sf::View();
        state.camera.setSize({1920, 1080});

        state.window.setView(state.camera);

        state.clock = sf::Clock();
        
        state.scene = Game::Scene::MainMenu;
        state.running = true;     
        
        Map::load("assets/map.png");

        state.mainmenu = {
            "Wendy's Basement",
            "Play",
            "Quit",
            "assets/menu_background.png"
        };

        state.pausemenu = {
            "Paused",
            "Resume",
            "Main Menu",
            "assets/pause_background.png"
        };

        DEBUG("GAME: Initialized game state.");
    }


    void start(State& state) {

        state.scene = Game::Scene::Game;

        state.player = Player::Entity({1200, 880});
        state.ghost = Ghost::Entity({100, 400});

        DEBUG("GAME: Starting game, switched to Game scene.");
    }

    void close(State& state) {

        state.running = false;
        if (state.window.isOpen()) {
            state.window.close();
        }

        DEBUG("GAME: Bye Bye");
    }

    
    void handleEvents(State& state) {
        while (const auto event = state.window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                state.running = false;
            }

            if (const auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    if (state.scene == Game::Scene::Game) state.scene = Game::Scene::PauseMenu;
                }
            }
        }
    }

    void update(State& state) {

        state.dt = state.clock.restart().asSeconds();

        if (state.scene != Game::Scene::Game) return;

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

        if (Ghost::hasCaughtPlayer(state.ghost, state.player)) state.scene = Game::Scene::MainMenu;
    }


    void render(State& state) {

        state.window.clear();
        state.camera.setCenter(state.player.position);

        if (state.scene == Game::Scene::Game) {
            Graphics::drawMap(state.window);
            Graphics::drawGhost(state.window, state.ghost);
            Graphics::drawPlayer(state.window, state.player);
            Graphics::drawOverlay(state.window, state.camera, state.player);
        }
        else if (state.scene == Game::Scene::MainMenu) {
            int chosen = Graphics::showMenu(
                state.window,
                state.mainmenu
            );

            if (chosen == 1) {
                Game::start(state);
            }
            else if (chosen == 2) {
                Game::close(state);
            }
        }
        else {
            int chosen = Graphics::showMenu(
                state.window,
                state.pausemenu
            );

            if (chosen == 1) {
                state.scene = Game::Scene::Game;
            }
            else if (chosen == 2){
                state.scene = Game::Scene::MainMenu;
            }
        }
        
        state.window.display();
    }



    void run() {

        State state;
        Game::init(state);

        while (state.running) {
            Game::handleEvents(state);
            Game::update(state);
            Game::render(state);
        }

        Game::close(state);
    }
}