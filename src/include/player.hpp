#pragma once

#include <SFML/Graphics/CircleShape.hpp>

namespace Player {

    const float walking_speed = 500;
    const float running_speed = 900;

    struct Entity {
        sf::CircleShape shape;
        sf::Vector2f position;
        float size;
        float speed;
    };

    void updatePosition(Entity& player, float dt);
    bool hasCollided(Entity& player);
}