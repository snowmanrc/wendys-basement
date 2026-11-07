#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>

#include "player.hpp"

namespace Game {

    struct State {
        sf::RenderWindow window;
        sf::View camera;
        sf::Clock clock;
        bool running;
        float dt;
        Player::Entity player;
    };

    void init(State& state);
    void update(State& state);
    void handleEvents(State& state);
    void render(State& state);
    void end(State& state);

    void run();
};