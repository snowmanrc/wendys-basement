
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include<string>



#include "include/ghost.hpp"
#include "include/map.hpp"
#include "include/player.hpp"
#include "include/debug.hpp"


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
        triggerMenu(sf::RenderWindow& window, std::string title, std::string option1, std::string option2, std::string backgroundImage) {
        
        window.setView(window.getDefaultView()); // to attain original window view when ghost cathch the player
        sf::Texture backgroundTexture; // to load image in memory

        if (!backgroundTexture.loadFromFile(backgroundImage))
        {
            DEBUG("Failed to load background image");
            return 2;
        }

		sf::Sprite background(backgroundTexture);// to show image object on screen we need to create sprite and pass texture

        sf::Vector2u imageSize = backgroundTexture.getSize(); // getting image original height and width

		// setting background according to window size
        background.setScale(
            {   // dividing required size by original size to get scale then sfml multiply this scale with orginal size to get required size
                 720.f / static_cast<float>(imageSize.x),
                 600.f / static_cast<float>(imageSize.y)
            }
            );



        sf::Font font;

        if (!font.openFromFile("assets/Creepster-Regular.ttf"))
        {
            DEBUG("Failed to load font");
            return 2;
        }

        // craeting menu title
        sf::Text titleText(font, title, 48);

        titleText.setFillColor(sf::Color(160, 0, 24));// setting title color blood red(to make it make more creepy) these three numbers show intensity of color RGB 

        // for making title at center
        sf::FloatRect bounds = titleText.getLocalBounds();

        titleText.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });

        // placing a title at horizental center
        titleText.setPosition({ 360.f, 120.f });// 360 means half of width which is 720 and 120 means add 120 pixeils from verticle top(which is zero) in total height is 600

        //creating first button
        sf::RectangleShape button1({ 250.f, 70.f });
        button1.setPosition({ 235.f, 280.f });
        button1.setOutlineThickness(2.f);
        button1.setOutlineColor(sf::Color(160, 0, 24));

        // creating second button same like first just changing vertical position of button
        sf::RectangleShape button2({ 250.f, 70.f });
        button2.setPosition({ 235.f, 380.f });
        button2.setOutlineThickness(2.f);
        button2.setOutlineColor(sf::Color(160, 0, 24));

        //creating  button text
        sf::Text text1(font, option1, 32);
        sf::Text text2(font, option2, 32);

        text1.setFillColor(sf::Color(214, 208, 194)); // chosing old bane white color(because it looks more creepy)
        text2.setFillColor(sf::Color(214, 208, 194));

        //for making first button text at center like we do for title
        sf::FloatRect bounds1 = text1.getLocalBounds();

        text1.setOrigin({
            bounds1.position.x + bounds1.size.x / 2.f,
            bounds1.position.y + bounds1.size.y / 2.f
            });

        text1.setPosition({ 360.f, 315.f });// for height we half the box height then add it into starting position height of the box

        //for second button text centering like above
        sf::FloatRect bounds2 = text2.getLocalBounds();

        text2.setOrigin({
            bounds2.position.x + bounds2.size.x / 2.f,
            bounds2.position.y + bounds2.size.y / 2.f
            });

        text2.setPosition({ 360.f, 415.f });

        //intially selecting first button
        int selected = 0;

        //main loop for menu
        while (window.isOpen())
        {
            //loop checking availability of events
            while (const auto event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                    return 2;
                }

                // taking input from keyboard
                if (const auto key = event->getIf<sf::Event::KeyPressed>())// if mouse event happen then this condition become false due to getif
                {
                    if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::Down)
                    {
                        selected = 1 - selected; // now selection goes to other button
                    }

                    if (key->code == sf::Keyboard::Key::Enter)
                    {
                        return selected + 1; // Return 1 for button1, 2 for button2
                    }
                }
                // mouse movemnet 
                if (const auto mouse = event->getIf<sf::Event::MouseMoved>())
                {
                    sf::Vector2f mousePosition =
                        window.mapPixelToCoords(mouse->position); // using sfml built in function in which we convert mouse pixel position to viwe cordinates

                   
                    //cheking that mouse button is on the cursor or not
                    if (button1.getGlobalBounds().contains(mousePosition)) // cheeck area of button1 and check cursor lie on that area
                        selected = 0;

                    else if (button2.getGlobalBounds().contains(mousePosition))
                        selected = 1;
                }

                //mouse click event
                if (const auto mouse = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouse->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2f mousePosition =
							window.mapPixelToCoords(mouse->position);// same as above we convert mouse pixel position to viwe cordinates

                        

                        if (button1.getGlobalBounds().contains(mousePosition))
                            return 1;

                        if (button2.getGlobalBounds().contains(mousePosition))
                            return 2;
                    }
                }



            }



            // highlighting the selected button
            button1.setFillColor(selected == 0 ? sf::Color(100, 0, 15) : sf::Color(25, 25, 25));
            button2.setFillColor(selected == 1 ? sf::Color(100, 0, 15) : sf::Color(25, 25, 25));


            // Displaying the menu on screen
            window.clear(sf::Color(12, 12, 16));// clear window and show dark background
            window.draw(background);
            window.draw(titleText);
            window.draw(button1);
            window.draw(button2);
            window.draw(text1);
            window.draw(text2);

            window.display(); // to show the menu on screen
        }
        return 2;
    }
    
}