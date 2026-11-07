#include <iostream>

#include <SFML/Window/Keyboard.hpp>

#include "map.hpp"
#include "player.hpp"


void Player::updatePosition(Entity &player, float dt) {

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
}


bool Player::hasCollided(Entity& player) {

	float r = player.shape.getRadius();
	float px = player.position.x + r;
	float py = player.position.y + r;

	for (int i = 0; i < Map::height; i++) {
		for (int j = 0; j < Map::width; j++) {

			auto tile = Map::grid[i][j];
			
			if (tile == 0) continue;

			float x = j * Map::tilesize;
			float y = i * Map::tilesize;
			int w = Map::tilesize, h = Map::tilesize;
			
			// collision detection

			float closest_x = std::max(x, std::min(px, x + w));
			float closest_y = std::max(y, std::min(py, y + h));

			float dist_x = px - closest_x;
			float dist_y = py - closest_y;
			float dist_sq = (dist_x * dist_x) + (dist_y * dist_y);

			// optimization: not taking sqrt() because its an expensive operation

			bool has_collided = dist_sq <= (r * r);

			if (has_collided) return true;
		}
	}

	return false;
}