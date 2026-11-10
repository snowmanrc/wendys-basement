
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "include/ghost.hpp"
#include "include/map.hpp"
#include "include/player.hpp"


namespace Graphics {

    void drawMap(sf::RenderWindow& window) {

        for (int i = 0; i < Map::height; i++) {
            for (int j = 0; j < Map::width; j++) {

                auto tile = Map::grid[i][j];
                
                sf::Color color;

                if (tile == 0) {
                    continue;
                } else {
                    color = sf::Color(41, 59, 61, 255);
                }

                float x = j * Map::tilesize;
                float y = i * Map::tilesize;

                auto shape = sf::RectangleShape({Map::tilesize, Map::tilesize});
                shape.setFillColor(color);
                shape.setPosition({x, y});
                window.draw(shape);
            }
        }

    }


    void drawPlayer(sf::RenderWindow& window, Player::Entity& player) {
        player.shape.setPosition(player.position);
        player.shape.setFillColor(sf::Color::Red);
        window.draw(player.shape);
    }


    void drawGhost(sf::RenderWindow& window, Ghost::Entity& ghost) {
        ghost.shape.setPosition(ghost.position);
        ghost.shape.setFillColor(sf::Color::Magenta);
        window.draw(ghost.shape);
    }


    int
    triggerMenu(sf::RenderWindow& window, std::string title, std::string option1, std::string option2) {
        // title -> the top big text
        // option1 -> contents of button 1
        // option2 -> contents of button 2

        // return type:
        // if option1 chosen -> return 1
        // if option2 chosen -> return 2
        return 1; // or 2 depending on what's chosen
    };
    
}