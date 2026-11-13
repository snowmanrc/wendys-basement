#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include<string>

#include "player.hpp"
#include "ghost.hpp"


namespace Graphics {

    void drawMap(sf::RenderWindow& window);
    void drawPlayer(sf::RenderWindow& window, Player::Entity& player);
    void drawGhost(sf::RenderWindow& window, Ghost::Entity& ghost);

    int triggerMenu(sf::RenderWindow& window, std::string title, std::string option1, std::string option2, std::string backgroundImage);
}