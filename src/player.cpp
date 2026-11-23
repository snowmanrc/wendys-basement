#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <cmath>
#include <SFML/Window/Keyboard.hpp>

#include "include/debug.hpp"
#include "include/map.hpp"
#include "include/utils.hpp"
#include "include/player.hpp"


namespace Player {

	void updatePosition(Entity &player, float dt) {

		bool shift_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);
		

		if (!shift_pressed && player.stamina < Player::MAX_STAMINA) {
			player.speed = Player::WALKING_SPEED;
			player.stamina += 1;
		}

		if (player.hidden) return;

		sf::Vector2f direction{0,0};

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) 
			direction.y -= 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) 
			direction.x -= 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) 
			direction.y += 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) 
			direction.x += 1;
		// directions handled

		// early return so we don't have to do any computation when the player is not moving
		if (direction.x == 0 && direction.y == 0) return;

		if (shift_pressed && player.stamina > 0) {
			player.speed = Player::RUNNING_SPEED;
			player.stamina -= 1;
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

		DEBUG(
			"PLAYER: Position: Coordinate(%f, %f) Tiled(%i, %i)", 
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


	void tryHiding(Entity & player) {

		if (player.hidden) {
			player.hidden = false;
			DEBUG("PLAYER: Unhidden");
			return;
		}

		// 2. Get the player's current grid tile
		auto player_tile = Utils::cartesianToTiledVector(
			player.position + sf::Vector2f(player.size, player.size), 
			Map::tilesize
		);

		// 3. Check if any crate tile is adjacent (including diagonals)
		for (const auto& crate_tile : Map::crate_slots) {
			int dx = std::abs(player_tile.x - crate_tile.x);
			int dy = std::abs(player_tile.y - crate_tile.y);

			if (dx <= 1 && dy <= 1) {
				player.hidden = true;
				DEBUG("PLAYER: Hidden near crate at Tiled(%d, %d)", crate_tile.x, crate_tile.y);
				return;
			}
		}

		DEBUG("PLAYER: Not next to any crate slot");
	}





	void tryCollectingCross(Entity& player) {

		auto player_tile = Utils::cartesianToTiledVector(player.position, Map::tilesize);

		for (unsigned int i = 0; i < Map::cross_slots.size(); i++) {
			auto cross_tile = Map::cross_slots[i];
			if ((cross_tile - player_tile).lengthSquared() > 2) continue;
				
			Map::cross_slots.erase(Map::cross_slots.begin() + i);
			player.crosses_found++;
			DEBUG("PLAYER: Cross at Tiled(%d, %d) found. Key count: %d", cross_tile.x, cross_tile.y, player.crosses_found);
			break;
		}

	}

	bool hasWon(Entity& player) {
		return player.crosses_found == Map::crosscount;
	}
}