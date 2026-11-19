#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include<string>

#include "player.hpp"
#include "ghost.hpp"


namespace Graphics {

    void drawMap(sf::RenderWindow& window);
    void drawPlayer(sf::RenderWindow& window, Player::Entity& player);
    void drawGhost(sf::RenderWindow& window, Ghost::Entity& ghost);

    
    
    void drawOverlay(sf::RenderWindow& window, sf::View& camera, Player::Entity& player);
    int triggerMenu(sf::RenderWindow& window, std::string title, std::string option1, std::string option2, std::string backgroundImage);
}