#include "include/map.hpp"
#include "include/debug.hpp"
#include <iostream>
#include<SFML/Graphics/Image.hpp>


namespace Map {
    
    int grid[width][height] = {};

    void load(std::string filepath) {

        sf::Image image;
        

        if (!image.loadFromFile(filepath))
        {
           std:: cout << "Image could not be loaded!" <<std:: endl;
            return;
        }

		sf::Vector2u size = image.getSize();

        for (int i = 0; i < size.y; i++) {
            for (int j = 0; j < size.x; j++) {

				sf::Color pixel = image.getPixel( { (unsigned int)j, (unsigned int )i});

                if (pixel == sf::Color::White)
                {
                    grid[i][j] = 0;
                }
                else if (pixel == sf::Color::Black)
                {
                    grid[i][j] = 1;
                    DEBUG("%i %i", i, j);
				}
				
               
            }
        }
       
		
   
    }
    
}
