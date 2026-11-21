
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
#include <SFML/Window/Keyboard.hpp>
#include <type_traits>

#include "include/game.hpp"
#include "include/debug.hpp"
#include "include/ghost.hpp"
#include "include/map.hpp"
#include "include/player.hpp"
#include "include/graphics.hpp"
#include "include/sound.hpp"
#include "include/utils.hpp"


namespace Game {

    State init() {

        auto main_menu = Graphics::makeMenu("Wendy's Basement", "Play", "Quit", "assets/menu_background.png");
        auto pause_menu = Graphics::makeMenu("Paused", "Resume", "Main Menu", "assets/pause_background.png");

        State state = {
            sf::RenderWindow(sf::VideoMode({720, 600}), "Wendy's Basement"),
            sf::View(),
            sf::Clock(),
            Game::Scene::MainMenu,
            {},
            {},
            main_menu,
            pause_menu,
            true,
            0
        };

        state.window.setFramerateLimit(120);
        state.camera.setSize({1920, 1080});

        bool loaded = Map::load("assets/map.png");

        if (!loaded) {
            DEBUG("GAME: ERROR: Failed to load map, cannot start game!");
            exit(1);
        }

        DEBUG("GAME: Initialized game state.");

        return state;
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
                    if (state.scene == Game::Scene::Game) state.scene = Scene::PauseMenu;
                }

                if (state.scene == Scene::MainMenu || state.scene == Scene::PauseMenu) {
                    auto& menu = state.scene == Game::Scene::MainMenu ? state.main_menu: state.pause_menu;

                    if (key->code == sf::Keyboard::Key::Num1) Graphics::selectMenuOption(menu, 0);
                    if (key->code == sf::Keyboard::Key::Num2) Graphics::selectMenuOption(menu, 1);
                    if (key->code == sf::Keyboard::Key::Enter) Graphics::markChosenMenuOption(menu);
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

        if (state.scene == Game::Scene::Game) {
            state.camera.setCenter(state.player.position);
            state.window.setView(state.camera);

            Graphics::drawMap(state.window);
            Graphics::drawGhost(state.window, state.ghost);
            Graphics::drawPlayer(state.window, state.player);
            Graphics::drawOverlay(state.window, state.camera, state.player);
        }
        else if (state.scene == Game::Scene::MainMenu) {
            state.camera.setCenter({ 960, 540 });
            state.window.setView(state.camera);

            Graphics::showMenu(state.window, state.main_menu);
            int chosen = Graphics::getChosenMenuOption(state.main_menu);

            if (chosen == 0) Game::start(state);
            else if (chosen == 1) state.running = false;
        }
        else if (state.scene == Game::Scene::PauseMenu) {
            state.camera.setCenter({ 960, 540 });
            state.window.setView(state.camera);

            Graphics::showMenu(state.window, state.pause_menu);
            int chosen = Graphics::getChosenMenuOption(state.pause_menu);

            if (chosen == 0) state.scene = Game::Scene::Game;
            else if (chosen == 1) state.scene = Game::Scene::MainMenu;
        }

        
        state.window.display();
    }



    void run() {

        State state = Game::init();

        while (state.running) {
            Game::handleEvents(state);
            Game::update(state);
            Game::render(state);
        }

        Game::close(state);
    }
}