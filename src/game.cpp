
#include <SFML/Audio/Music.hpp>
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

        Sound::startMusic(1);

        DEBUG("GAME: Initialized game state.");
        
        return state;
    }


    void start(State& state) {

        state.player = Player::Entity({1200, 880});
        state.ghost = Ghost::Entity({100, 400});

        Sound::startMusic(0);
        Sound::pauseMusic();

        DEBUG("GAME: Game startup complete");
    }


    void changeScene(State& state, Scene scene) {
        
        state.scene = scene;

        if (scene == Game::Scene::Playing) {
            Sound::playMusic();
        }
        else if (scene == Game::Scene::PauseMenu) {
            Sound::pauseMusic();
        }
        else if (scene == Game::Scene::MainMenu) {
            Sound::startMusic(1);
        }
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
                    if (state.scene == Game::Scene::Playing) Game::changeScene(state, Scene::PauseMenu);
                }


                if (state.scene == Scene::MainMenu || state.scene == Scene::PauseMenu) {
                    auto& menu = state.scene == Game::Scene::MainMenu ? state.main_menu: state.pause_menu;

                    if (key->code == sf::Keyboard::Key::Num1) Graphics::selectMenuOption(menu, 0);
                    if (key->code == sf::Keyboard::Key::Num2) Graphics::selectMenuOption(menu, 1);
                    if (key->code == sf::Keyboard::Key::Enter) Graphics::markChosenMenuOption(menu);
                }
            }

            if (const auto key = event->getIf<sf::Event::KeyReleased>()) {

                if (key->code == sf::Keyboard::Key::E) {
                    Player::tryHiding(state.player);
                }
            }
        }
    }

    void update(State& state) {

        state.dt = state.clock.restart().asSeconds();

        if (state.scene != Game::Scene::Playing) return;

        Player::updatePosition(state.player, state.dt);       
        
        static sf::Clock path_finding_clock;
        sf::Time elapsed = path_finding_clock.getElapsedTime();

        if (elapsed >= sf::seconds(0.1f)) {
            Ghost::decideTarget(state.ghost, state.player);
            // Reset the clock for the next interval
            path_finding_clock.restart();
        }
        
        Ghost::updatePosition(state.ghost, state.dt);

        if (Ghost::hasCaughtPlayer(state.ghost, state.player)) {
            Game::changeScene(state, Scene::MainMenu);
        }
    }


    void render(State& state) {

        state.window.clear();

        if (state.scene == Game::Scene::Playing) {
            state.camera.setCenter(state.player.position);
            state.window.setView(state.camera);

            Graphics::drawMap(state.window);
            Graphics::drawGhost(state.window, state.ghost);
            Graphics::drawPlayer(state.window, state.player);
            Graphics::drawOverlay(state.window, state.camera, state.player);

            std::string texts[] = {"Stamina", "Keys Left"};
            int values[] = {100, 5};
            int count = sizeof(values)/sizeof(values[0]);

            Graphics::drawHUD(state.window, texts, values, count);
        }
        else if (state.scene == Game::Scene::MainMenu) {
            state.camera.setCenter({ 960, 540 });
            state.window.setView(state.camera);
            Graphics::showMenu(state.window, state.main_menu);
            int chosen = Graphics::getChosenMenuOption(state.main_menu);

            if (chosen == 0) {
                Game::start(state);
                Game::changeScene(state, Scene::Playing);
            }
            else if (chosen == 1) {
                state.running = false;
            }
        }
        else if (state.scene == Game::Scene::PauseMenu) {
            state.camera.setCenter({ 960, 540 });
            state.window.setView(state.camera);

            Graphics::showMenu(state.window, state.pause_menu);
            int chosen = Graphics::getChosenMenuOption(state.pause_menu);

            if (chosen == 0) {
                Game::changeScene(state, Scene::Playing);
            }
            else if (chosen == 1) {
                Game::changeScene(state, Scene::MainMenu);
            }
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