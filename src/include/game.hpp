#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>

#include "graphics.hpp"
#include "player.hpp"
#include "ghost.hpp"


namespace Game {

    enum class Scene {
        MainMenu,
        PauseMenu,
        Playing
    };

    struct State {
        sf::RenderWindow window;
        sf::View camera;
        sf::Clock clock;
        Scene scene;
        Player::Entity player;
        Ghost::Entity  ghost;
        Graphics::Menu main_menu;
        Graphics::Menu pause_menu;
        bool running;
        float dt;
    };

    // Initializes the Game and setups the window
    State init();

    // Closes the game
    void close(State& state);

    // Starts a game scene
    void start(State& state);

    // Sets up the game scene
    void changeScene(State& state, Scene scene);

    // Updates the game state
    void update(State& state);

    // Handles all global events
    void handleEvents(State& state);

    // Triggers the rendering of the game
    void render(State& state);
    
    // Main game loop
    void run();
};