#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "player.hpp"
#include "ghost.hpp"


namespace Graphics {

    void drawMap(sf::RenderWindow& window);
    void drawPlayer(sf::RenderWindow& window, Player::Entity& player);
    void drawGhost(sf::RenderWindow& window, Ghost::Entity& ghost);

}