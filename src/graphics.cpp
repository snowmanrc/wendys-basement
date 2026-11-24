
#include "include/graphics.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <map>
#include<string>



#include "include/error.hpp"
#include "include/ghost.hpp"
#include "include/map.hpp"
#include "include/player.hpp"
#include "include/debug.hpp"
#include "include/utils.hpp"


namespace Graphics {

    static std::map<std::string, sf::Texture> textures;


    static inline const sf::Texture&
    getTexture(std::string path) {
        if (Graphics::textures.contains(path)) {
            return Graphics::textures[path];
        }

        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            ERROR("GRAPHICS: ERROR: Failed to load asset %s", path.data());
        }
        // failure here
        Graphics::textures[path] = texture;
        return Graphics::textures[path];
    }


    void drawMap(sf::RenderWindow& window) {

        auto texture = Graphics::getTexture(Assets::wall_sprite);
        auto wall = sf::Sprite(texture);
        wall.setScale({
            Map::tilesize / static_cast<float>(texture.getSize().x),
            Map::tilesize / static_cast<float>(texture.getSize().y)
        });

        auto texture2 = Graphics::getTexture(Assets::floor_sprite);
        auto floor = sf::Sprite(texture2);
        floor.setScale({
            Map::tilesize / static_cast<float>(texture2.getSize().x),
            Map::tilesize / static_cast<float>(texture2.getSize().y)
        });

        for (int i = 0; i < Map::height; i++) {
            for (int j = 0; j < Map::width; j++) {

                auto tile = Map::grid[i][j];
                
                if (tile == 0) {
                    floor.setPosition(Utils::tiledToCartesianVector({j, i}, Map::tilesize));
                    window.draw(floor);
                } else {
                    wall.setPosition(Utils::tiledToCartesianVector({j, i}, Map::tilesize));
                    window.draw(wall);
                }

                
            }
        }

    }


    void drawPlayer(sf::RenderWindow& window, Player::Entity& player) {

        if (player.hidden) return;

        auto texture = Graphics::getTexture(Assets::player_sprite);
        auto sprite = sf::Sprite(texture);
        sprite.setScale({
            player.size / static_cast<float>(texture.getSize().x) * 2.5f,
            player.size / static_cast<float>(texture.getSize().y) * 2.5f
        });
        sprite.setPosition(player.position);
        window.draw(sprite);
    }


    void drawGhost(sf::RenderWindow& window, Ghost::Entity& ghost) {
        auto texture = Graphics::getTexture(Assets::ghost_sprite);
        auto sprite = sf::Sprite(texture);
        sprite.setScale({
            ghost.size / static_cast<float>(texture.getSize().x) * 3.5f,
            ghost.size / static_cast<float>(texture.getSize().y) * 3.5f
        });
        sprite.setPosition(ghost.position);
        window.draw(sprite);
    }

    
    void drawOverlay(sf::RenderWindow& window, sf::View& camera, Player::Entity& player) {

        static sf::Texture texture;
        static bool loaded = texture.loadFromFile(Assets::overlay_image);

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
        sprite.setPosition(
            player.position + sf::Vector2f(player.size, player.size) - sf::Vector2f(camera.getSize().x/2.0f, camera.getSize().y/2.0f)
        );
        window.draw(sprite, states);  
    }

    void drawHUD(sf::RenderWindow& window, std::string texts[], int values[], int count) 
    {
		static sf::Font font;
		static bool font_loaded = false;

        if (!font_loaded)
        {
            if (!font.openFromFile(Assets::silkscreen_regular_font))
            {
                DEBUG("Failed to load HUD font");

                return;
            }

            font_loaded = true;


        }

        sf::View oldView = window.getView();
        auto windowSize = window.getSize();

		//creating view according to window size to make HUD at fixed position
        sf::View hudView(sf::FloatRect({ 0.f, 0.f }, { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) }));
        window.setView(hudView);

        for (int i = 0; i < count; i++)
        {
            std::string text = texts[i];
            int num = values[i];

            sf::Text hudText(font, text + ": " + std::to_string(num), 32);
            hudText.setFillColor(sf::Color(214, 208, 194));

            // setting the position  ogf the text with each next line 45 pixel lower
            hudText.setPosition({  30.f, 30.f + i * 45.f });

            window.draw(hudText);
        }
        //Restoring the game camera view
        window.setView(oldView);

    }

    void drawProps(sf::RenderWindow& window) {

        for (unsigned int i = 0; i < Map::cross_slots.size(); i++) {
            auto tile = Map::cross_slots[i];
            auto texture = Graphics::getTexture(Assets::cross_sprite);
            sf::Sprite sprite(texture);
            sprite.setScale({3.5,3.5});
            sprite.setPosition(Utils::tiledToCartesianVector(tile, Map::tilesize));
            window.draw(sprite);
        }

        for (unsigned int i = 0; i < Map::crate_slots.size(); i++) {
            auto tile = Map::crate_slots[i];
            auto texture = Graphics::getTexture(Assets::crate_sprite);
            sf::Sprite sprite(texture);
            sprite.setScale(
                {
                    (float) Map::tilesize / texture.getSize().x, 
                    (float) Map::tilesize / texture.getSize().y
                }
            );
            sprite.setPosition(Utils::tiledToCartesianVector(tile, Map::tilesize));
            window.draw(sprite);
        }
    }


    Menu makeMenu(std::string t, std::string o1, std::string o2, std::string bg, std::string o3) {

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
            o3,
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
            if (!font.openFromFile(Assets::silkscreen_regular_font)) {
                DEBUG("Failed to load font");
                exit(1);
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
        sf::Text title_text(font, menu.title, 128);

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
    
        //creating first button
        sf::RectangleShape button1({ 500.f, 60.f });
        button1.setFillColor(sf::Color(0,0,0));
        button1.setPosition({ 710.f, 480.f });
        button1.setOutlineThickness(6.f);
        button1.setOutlineColor(menu.selected == 0 ? sf::Color(214, 208, 194): sf::Color(160, 0, 24));
    
        // creating second button same like first just changing vertical position of button
        sf::RectangleShape button2({ 500.f, 60.f });
        button2.setFillColor(sf::Color(0,0,0));
        button2.setPosition({ 710.f, 580.f });
        button2.setOutlineThickness(6.f);
        button2.setOutlineColor(menu.selected == 1 ? sf::Color(214, 208, 194): sf::Color(160, 0, 24));

        //creating third button for game guide just for main menu
        if (menu.option3 != "") {
            sf::RectangleShape button3({ 500.f, 60.f });
            button3.setFillColor(sf::Color(0, 0, 0));
            button3.setPosition({ 710.f, 680.f });
            button3.setOutlineThickness(6.f);
            button3.setOutlineColor(menu.selected == 2 ? sf::Color(214, 208, 194) : sf::Color(160, 0, 24));

            sf::Text text3(font, menu.option3, 32);
            text3.setFillColor(sf::Color(214, 208, 194));

            sf::FloatRect bounds3 = text3.getLocalBounds();
            text3.setOrigin({
                bounds3.position.x + bounds3.size.x / 2.f,
                bounds3.position.y + bounds3.size.y / 2.f
                });

            text3.setPosition({ 960.f, 710.f });

            window.draw(button3);
            window.draw(text3);
        }
    
        //creating  button text
        sf::Text text1(font, menu.option1, 32);
        sf::Text text2(font, menu.option2, 32);

        
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

        text1.setPosition({ 960.f, 510.f });
        text2.setPosition({ 960.f, 610.f });
    
    
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
            return -1;
        }
    }
    
    void selectMenuOption(Menu& menu, int option) {
        if (menu.option3 != "") {
            menu.selected = option % 3;
        }
        else {
            menu.selected = option % 2;
        }

    }

    void markChosenMenuOption(Menu& menu) {
        menu.chosen = true;
    }


    bool showGameGuide(sf::RenderWindow& window) {

        static sf::Font font;
        static bool font_loaded = false;

        if (!font_loaded) {
            if (!font.openFromFile(Assets::silkscreen_regular_font)) {
                ERROR("Failed to load Game Guide font");
            }
            font_loaded = true;
        }

        auto& texture = Graphics::getTexture(Assets::menu_bg_image);
        sf::Sprite background(texture);

        auto view_size = window.getView().getSize();
        auto image_size = texture.getSize();

        background.setScale({
            view_size.x / static_cast<float>(image_size.x),
            view_size.y / static_cast<float>(image_size.y)
        });

        window.draw(background);

        sf::Text title(font, "HOW TO PLAY", 64);
        title.setFillColor(sf::Color(160, 0, 24));

        sf::FloatRect title_bounds = title.getLocalBounds();
        title.setOrigin({
            title_bounds.position.x + title_bounds.size.x / 2.f,
            title_bounds.position.y + title_bounds.size.y / 2.f
        });

        title.setPosition({ 960.f, 250.f });

        sf::Text instructions(font,
            "You're trapped in Wendy's Basement!\n"
            "To escape, you must collect all 5 crosses! \n"
            "Be ware of Wendy, she is searching for you! \n"
            "+ W or Arrow Up - To Move Up\n"
            "+ A or Arrow Left - To Move Left\n"
            "+ S or Arrow Down - To Move Down\n"
            "+ D or Arrow Right - To Move Right\n"
            "+ Shift - To run so Wendy can't catch you!\n"
            "+ E - To hide inside a crate, \n"
            "so Wendy doesn't spot you!",
            32
        );

        instructions.setFillColor(sf::Color(214, 208, 194));
        instructions.setPosition({ 600.f, 400.f });

        sf::Text back_text(font, "Press ESC to return to Main Menu", 24);
        
        back_text.setFillColor(sf::Color(214, 208, 194));

        sf::FloatRect back_bounds = back_text.getLocalBounds();
        back_text.setOrigin({
            back_bounds.position.x + back_bounds.size.x / 2.f,
            back_bounds.position.y + back_bounds.size.y / 2.f
        });

        back_text.setPosition({ 960.f, 900.f });

        window.draw(title);
        window.draw(instructions);
        window.draw(back_text);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) return true;
        return false;
    }

}