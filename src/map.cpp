#include "include/map.hpp"
#include <iostream>


namespace Map {
    
    int grid[width][height] = {};

    void load(std::string filepath) {

        for (int i = 0; i < Map::height; i++) {
            for (int j = 0; j < Map::width; j++) {
    
                Map::grid[i][j] = std::rand() % 5 == 0 ? 1: 0;
            }
        }
    
        std::cout << "runs\n";
    
    }
    
}
