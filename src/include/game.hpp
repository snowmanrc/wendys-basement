#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>

#include "player.hpp"
#include "ghost.hpp"

namespace Game {

    struct State {
        bool closed;
        sf::RenderWindow window;
        sf::View camera;
        sf::Clock clock;
        bool running;
        float dt;
        Player::Entity player;
        Ghost::Entity  ghost;
    };

    void init(State& state);
    void start(State& state);
    void update(State& state);
    void handleEvents(State& state);
    void render(State& state);
    void end(State& state);

    void run();
};