#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

namespace Map {

    const int width = 100;
    const int height = 100;
    const float tilesize = 64;

    extern std::vector<sf::Vector2i> key_slots;
    extern std::vector<sf::Vector2i> ghost_idle_spots;
    extern sf::Vector2i player_spawn_pos;

    extern int grid[width][height];

    void load(std::string filepath);
}