#pragma once

#include <string>

namespace Map {

    const int width = 100;
    const int height = 100;
    const float tilesize = 64;

    extern int grid[width][height];

    void load(std::string filepath);
}