#include <iostream>

#include <SFML/Window/Keyboard.hpp>

#include "include/debug.hpp"
#include "include/map.hpp"
#include "include/utils.hpp"
#include "include/player.hpp"


namespace Player {

	void updatePosition(Entity &player, float dt) {

		sf::Vector2f direction{0,0};

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) direction.y -= 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction.x -= 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) direction.y += 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction.x += 1;

		if (
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || 
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)
		) {
			player.speed = Player::running_speed;
		} else {
			player.speed = Player::walking_speed;
		}

		// early return so we don't have to do any computation when the player is not moving
		if (direction.x == 0 && direction.y == 0) return;


		/* 
		because both have magnitude, the movement will be along the hypotenuse right?
		so the speed would be boosted by a factor of sqrt(2) = 1.414
		to fix this we divide both the coordinates by sqrt(2) so the hypotenuse is 1 and speed is balanced
		*/

		if (direction.x && direction.y) {
			direction.x /= 1.414;
			direction.y /= 1.414;
		}

		auto old_pos = player.position;

		player.position.x += direction.x * player.speed * dt;
		if (Player::hasCollided(player)) {
			player.position.x = old_pos.x;
		}

		player.position.y += direction.y * player.speed * dt;
		if (Player::hasCollided(player)) {
			player.position.y = old_pos.y;
		}

		DEBUG(
			"Player Position: Coordinate(%f, %f) Tiled(%i, %i)", 
			player.position.x, 
			player.position.y,
			(int)(player.position.x / Map::tilesize),
			(int)(player.position.y / Map::tilesize)
		);
	}


	bool hasCollided(Entity& player) {
        
		float cr = player.shape.getRadius();
		float cx = player.position.x + cr;
		float cy = player.position.y + cr;

		for (int i = 0; i < Map::height; i++) {
			for (int j = 0; j < Map::width; j++) {

				auto tile = Map::grid[i][j];
				
				if (tile == 0) continue;

				float rx = j * Map::tilesize;
				float ry = i * Map::tilesize;
				int rw = Map::tilesize, rh = Map::tilesize;
				
				bool collided = Utils::detectCircleRectCollision(cr, cx, cy, rx, ry, rw, rh);
				

				if (collided) return true;
			}
		}

		return false;
    }


}