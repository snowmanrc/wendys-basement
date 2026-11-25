#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace Player {

    const float WALKING_SPEED = 500;
    const float RUNNING_SPEED = 900;

    const float SIZE = 24;
    const unsigned int MAX_STAMINA = 1000;

    struct Entity {
        sf::CircleShape shape;
        sf::Vector2f position;
        float size;
        float speed;
        bool hidden;
        unsigned int crosses_found;
        unsigned int stamina;

        Entity() = default;
        Entity(sf::Vector2f p):
        shape(sf::CircleShape(SIZE)), position(p), size(SIZE), speed(WALKING_SPEED), hidden(false), crosses_found(0), stamina(MAX_STAMINA) {}
    };

    void updatePosition(Entity& player, float dt);
    bool hasCollided(Entity& player);
    void tryHiding(Entity& player);
    void tryCollectingCross(Entity& player);
    bool hasWon(Entity& player);
    int findNearestCrossDistance(Entity& player);
}