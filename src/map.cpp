#include "include/map.hpp"
#include "include/debug.hpp"
#include <SFML/Graphics/Color.hpp>
#include <iostream>
#include<SFML/Graphics/Image.hpp>
#include <cstdlib>  
#include <ctime>    
#include <utility>


namespace Map {
    
    std::vector<sf::Vector2i> cross_slots;
    std::vector<sf::Vector2i> ghost_idle_spots;
	std::vector<sf::Vector2i> crate_slots;
    std::vector<sf::Vector2i> all_cross_slots;
    sf::Vector2i player_spawn_pos;

    int grid[width][height] = {};

    bool load(std::string filepath) {

        sf::Image image;
        
        if (!image.loadFromFile(filepath)) {
            DEBUG("Image could not be loaded!");
            return false;
        }
        //if map is load again then clear the previous data
        cross_slots.clear();
        ghost_idle_spots.clear();
        crate_slots.clear();
        all_cross_slots.clear();

		sf::Vector2u size = image.getSize();

        for (unsigned int i = 0; i < size.y; i++) {
            for (unsigned int j = 0; j < size.x; j++) {

				sf::Color pixel = image.getPixel( { (unsigned int) j, (unsigned int) i});
                
                if (pixel == sf::Color::Black) {
                    grid[i][j] = 1;
                    continue;
                }

                grid[i][j] = 0;

                if (pixel == sf::Color::Yellow) {
                   all_cross_slots.push_back(sf::Vector2i(j, i));
				}
                else if (pixel == sf::Color::Blue)
                {
                  ghost_idle_spots.push_back(sf::Vector2i(j, i));
                }
                else if (pixel == sf::Color::Magenta)
                {
                  crate_slots.push_back(sf::Vector2i(j, i));
                }
                else if (pixel == sf::Color::Green)
                {
                    player_spawn_pos = sf::Vector2i(j, i);
                }
            }
        }

		// Randomly select cross slots from all_cross_slots
        static bool randomStarted = false;

        if (!randomStarted)
        {
            srand(time(0));
            randomStarted = true;
        }

        int total = all_cross_slots.size();
        
        for (int i = 0; i < crosscount; i++)
        {
            // choosing random positions from the remaining positions
            int randomIndex = i + rand() % (total - i);

            cross_slots.push_back(all_cross_slots[randomIndex]);

            // swapping the selected position to avoid seleting it again
            std::swap(all_cross_slots[i], all_cross_slots[randomIndex]);
        }

        return true;
    }
}
