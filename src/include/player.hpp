#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace Player {

    const float WALKING_SPEED = 500;
    const float RUNNING_SPEED = 900;

    const float SIZE = 24;

    struct Entity {
        sf::CircleShape shape;
        sf::Vector2f position;
        float size;
        float speed;

        Entity() = default;
        Entity(sf::Vector2f p):
        shape(sf::CircleShape(SIZE)), position(p), size(SIZE), speed(WALKING_SPEED) {}
    };

    void updatePosition(Entity& player, float dt);
    bool hasCollided(Entity& player);
}