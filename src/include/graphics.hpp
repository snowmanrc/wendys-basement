#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <map>
#include<string>

#include "player.hpp"
#include "ghost.hpp"


namespace Graphics {

    void drawMap(sf::RenderWindow& window);

    void drawPlayer(sf::RenderWindow& window, Player::Entity& player);

    void drawGhost(sf::RenderWindow& window, Ghost::Entity& ghost);

    void drawOverlay(sf::RenderWindow& window, sf::View& camera, Player::Entity& player);

    void drawHUD(sf::RenderWindow& window, std::string texts[], int values[], int count);

    void drawProps(sf::RenderWindow& window);

    struct Menu {
        std::string title;
        std::string option1;
        std::string option2;
        std::string option3;
        std::string bg_path;
        int selected;
        bool chosen;
        bool bg_loaded;
    };

    Menu makeMenu(std::string t, std::string o1, std::string o2, std::string bg, std::string o3 = "");
    void showMenu(sf::RenderWindow& window, Menu& menu);
    void showGameGuide(sf::RenderWindow& window);
    int getChosenMenuOption(Menu& menu);
    void selectMenuOption(Menu& menu, int option);
    void markChosenMenuOption(Menu& menu);
}