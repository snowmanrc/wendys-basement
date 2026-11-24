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

    namespace Assets {
        // Images & Sprites
        const std::string menu_bg_image = "assets/bg.jpg";
        const std::string overlay_image = "assets/overlay.png";
        const std::string crate_sprite = "assets/crate.png";
        const std::string cross_sprite = "assets/cross.png";
        const std::string floor_sprite = "assets/floor.png";
        const std::string ghost_sprite = "assets/ghost.png";
        const std::string player_sprite = "assets/player.png";
        const std::string wall_sprite = "assets/wall.png";
    
        // Map Data
        const std::string map_file = "assets/map.png";
    
        // Audio
        const std::string faint_signal_music = "assets/faint-signal.mp3";
        const std::string golden_wind_music = "assets/golden-wind.mp3";
        const std::string old_doll_slowed_music = "assets/old_doll_slowed.mp3";
    
        // Fonts
        const std::string silkscreen_regular_font = "assets/silkscreen-regular.ttf";
    }

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
    int getChosenMenuOption(Menu& menu);
    void selectMenuOption(Menu& menu, int option);
    void markChosenMenuOption(Menu& menu);

    bool showGameGuide(sf::RenderWindow& window);
}