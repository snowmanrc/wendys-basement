#include <SFML/System/Vector2.hpp>

#include "include/ghost.hpp"
#include "include/map.hpp"

namespace Ghost {

    void updatePosition(Entity& ghost, float dt) {
        
        if (ghost.path.empty()) return;

        auto target_tile = ghost.path.back();
        sf::Vector2i ghost_tile = {
            static_cast<int>(ghost.position.x / Map::tilesize), 
            static_cast<int>(ghost.position.y / Map::tilesize)
        };

        if (target_tile == ghost_tile) {
            ghost.path.pop_back();
            return;
        }

        auto dir_vec = target_tile - ghost_tile;
        sf::Vector2f dir_vecf = {(float)dir_vec.x, (float)dir_vec.y}; 
        auto normalized_vec = dir_vecf.normalized();

        ghost.position.x += normalized_vec.x * ghost.speed * dt; 
        ghost.position.y += normalized_vec.y * ghost.speed * dt; 
    }
}