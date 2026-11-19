
#include "include/graphics.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <map>
#include<string>



#include "include/ghost.hpp"
#include "include/map.hpp"
#include "include/player.hpp"
#include "include/debug.hpp"


namespace Graphics {

    static std::map<std::string, sf::Texture> textures;

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

    
    void drawOverlay(sf::RenderWindow& window, sf::View& camera, Player::Entity& player) {

        static sf::Texture texture;
        static bool loaded = texture.loadFromFile("assets/overlay.jpg");

        if (!loaded) {
            DEBUG("GRAPHICS: WARNING: overlay file not found!");
            return;
        }

        sf::Sprite sprite(texture);
        sf::RenderStates states;
        states.blendMode = sf::BlendMultiply;
        sprite.setScale(
            {
                (float) camera.getSize().x / texture.getSize().x, 
                (float) camera.getSize().y / texture.getSize().y
            }
        );
        sprite.setPosition(player.position - sf::Vector2f(camera.getSize().x/2.0f, camera.getSize().y/2.0f));
        window.draw(sprite, states);  
    }


    Menu makeMenu(std::string t, std::string o1, std::string o2, std::string bg) {

        sf::Texture texture; 
        bool bg_loaded = texture.loadFromFile(bg);

        if (!bg_loaded) {
            DEBUG("GRAPHICS: Failed to load menu background image");
        };

        Graphics::textures[bg] = texture;
    
        Menu menu = {
            t,
            o1,
            o2,
            bg,
            0,
            false,
            bg_loaded
        };

        return menu;
    }


    void showMenu(sf::RenderWindow& window, Menu& menu) {

        static sf::Font font;
        static bool font_loaded = false;

        if (!font_loaded) {
            if (!font.openFromFile("assets/Creepster-Regular.ttf")) {
                DEBUG("Failed to load font");
            }
            font_loaded = true;
        }

        auto view_size = window.getView().getSize();

        if (menu.bg_loaded) {
            auto& texture = Graphics::textures.at(menu.bg_path);
            sf::Sprite background(texture);
            auto image_size = texture.getSize();
            background.setScale({   
                view_size.x / static_cast<float>(image_size.x),
                view_size.y / static_cast<float>(image_size.y)
            });
            window.draw(background);
        }

        
        // craeting menu title
        sf::Text title_text(font, menu.title, 64);

        title_text.setFillColor(sf::Color(160, 0, 24));
        // setting title color blood red(to make it make more creepy) these three numbers show intensity of color RGB 
    
        // for making title at center
        sf::FloatRect bounds = title_text.getLocalBounds();
    
        title_text.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });
    
        // placing a title at horizental center
        title_text.setPosition({ 960.f, 250.f });
        // 360 means half of width which is 720 and 120 means add 120 pixeils from verticle top(which is zero) in total height is 600
    
        //creating first button
        sf::RectangleShape button1({ 500.f, 120.f });
        button1.setPosition({ 710.f, 480.f });
        button1.setOutlineThickness(2.f);
        button1.setOutlineColor(sf::Color(160, 0, 24));
    
        // creating second button same like first just changing vertical position of button
        sf::RectangleShape button2({ 500.f, 120.f });
        button2.setPosition({ 710.f, 640.f });
        button2.setOutlineThickness(2.f);
        button2.setOutlineColor(sf::Color(160, 0, 24));
    
        //creating  button text
        sf::Text text1(font, menu.option1, 24);
        sf::Text text2(font, menu.option2, 24);

        
    
        text1.setFillColor(sf::Color(214, 208, 194)); 
        // chosing old bane white color(because it looks more creepy)
        text2.setFillColor(sf::Color(214, 208, 194));
    
        //for making first button text at center like we do for title
        sf::FloatRect bounds1 = text1.getLocalBounds();
    
        text1.setOrigin({
            bounds1.position.x + bounds1.size.x / 2.f,
            bounds1.position.y + bounds1.size.y / 2.f
        });
    
        // for height we half the box height then add it into starting position height of the box
    
        //for second button text centering like above
        sf::FloatRect bounds2 = text2.getLocalBounds();
    
        text2.setOrigin({
            bounds2.position.x + bounds2.size.x / 2.f,
            bounds2.position.y + bounds2.size.y / 2.f
        });

        text1.setPosition({ 960.f, 540.f });
        text2.setPosition({ 960.f, 700.f });
    
    
        // setting background according to window size
        // dividing required size by original size to get scale then sfml multiply this scale with orginal size to get required size
       
        window.draw(title_text);
        window.draw(button1);
        window.draw(button2);
        window.draw(text1);
        window.draw(text2);
    }


    int getChosenMenuOption(Menu& menu) {
        if (menu.chosen) {
            int selected = menu.selected;
            menu.selected = 0;
            menu.chosen = false;
            return selected;
        } else {
            return 0;
        }
    }
    
}