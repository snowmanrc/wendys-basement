#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>


namespace Ghost {

    struct Entity {
        sf::CircleShape shape;
        sf::Vector2f position;
        float size;
        float speed;
        std::vector<sf::Vector2i> path;
    };

    void updatePosition(Entity& ghost, float dt);
    bool hasCollided(Entity& ghost);
    void follow(Entity& ghost, sf::Vector2i target_tile);
    
    std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i end);
}