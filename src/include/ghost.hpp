#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

#include "player.hpp"


namespace Ghost {

    const float WALKING_SPEED = 500;
    const float RUNNING_SPEED = 800;

    const float SIZE = 26;

    enum class WalkingMode {
        Wandering, 
        Chasing
    };

    struct Entity {
        sf::CircleShape shape;
        sf::Vector2f position;
        float size;
        float speed;
        WalkingMode mode;
        std::vector<sf::Vector2i> path;

        Entity() = default;
        Entity(sf::Vector2f p):
        shape(sf::CircleShape(SIZE)), position(p), size(SIZE), speed(WALKING_SPEED), mode(WalkingMode::Wandering) {}
    };

    void updatePosition(Entity& ghost, float dt);
    bool hasCollided(Entity& ghost);
    bool hasCaughtPlayer(Entity& ghost, Player::Entity& player);

    void decideTarget(Entity& ghost, Player::Entity& player);

    void follow(Entity& ghost, sf::Vector2i target_tile);
    
    std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i end);
}