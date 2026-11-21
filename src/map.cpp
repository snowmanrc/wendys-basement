#include "include/map.hpp"
#include "include/debug.hpp"
#include <SFML/Graphics/Color.hpp>
#include <iostream>
#include<SFML/Graphics/Image.hpp>


namespace Map {
    
    std::vector<sf::Vector2i> key_slots;
    std::vector<sf::Vector2i> ghost_idle_spots;
    sf::Vector2i player_spawn_pos;

    int grid[width][height] = {};

    bool load(std::string filepath) {

        sf::Image image;
        
        if (!image.loadFromFile(filepath)) {
            DEBUG("Image could not be loaded!");
            return false;
        }

		sf::Vector2u size = image.getSize();

        for (unsigned int i = 0; i < size.y; i++) {
            for (unsigned int j = 0; j < size.x; j++) {

				sf::Color pixel = image.getPixel( { (unsigned int) j, (unsigned int) i});
                
                if (pixel == sf::Color::Black) {
                    grid[i][j] = 0;
                    continue;
                }

                grid[i][j] = 1;

                if (pixel == sf::Color::Yellow) {
                    // logic here
				}
            }
        }   

        return true;
    }
}
