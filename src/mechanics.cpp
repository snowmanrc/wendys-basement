#include "../include/definitions.hpp"
#include "../include/mechanics.hpp"

void updatePlayerPosition(GameState& state) {

    sf::Vector2f direction{0,0};

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) direction.y -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction.x -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) direction.y += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction.x += 1;

    /* 
    because both have magnitude, the movement will be along the hypotenuse right?
    so the speed would be boosted by a factor of sqrt(2) = 1.414
    to fix this we divide both the coordinates by sqrt(2) so the hypotenuse is 1 and speed is balanced
    */

	if (direction.x && direction.y) {
		direction.x /= 1.414;
		direction.y /= 1.414;
	}

	state.player.position += direction * state.player.speed * state.dt;

    state.camera.setCenter(state.player.position);
}