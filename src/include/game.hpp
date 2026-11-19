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
        Game
    };

    struct State {
        sf::RenderWindow window;
        sf::View camera;
        sf::Clock clock;
        Scene scene;
        Player::Entity player;
        Ghost::Entity  ghost;
        bool running;
        float dt;
    };

    // Initializes the Game and setups the window
    void init(State& state);

    // Closes the game
    void close(State& state);

    // Starts the Game
    void start(State& state);

    // Updates the game state
    void update(State& state);

    // Handles all global events
    void handleEvents(State& state);

    // Triggers the rendering of the game
    void render(State& state);
    
    // Main game loop
    void run();
};