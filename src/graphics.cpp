
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
                
                if (tile == 0) continue;

                float x = j * Map::tilesize;
                float y = i * Map::tilesize;

                auto shape = sf::RectangleShape({Map::tilesize, Map::tilesize});
                shape.setFillColor(sf::Color::Yellow);
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
    
}