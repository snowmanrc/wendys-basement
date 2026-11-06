#include "../include/definitions.hpp"
#include <iostream>
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

	auto old_pos = state.player.position;

	state.player.position.x += direction.x * state.player.speed * state.dt;
	if (hasPlayerCollided(state)) {
		state.player.position.x = old_pos.x;
	}

	state.player.position.y += direction.y * state.player.speed * state.dt;
	if (hasPlayerCollided(state)) {
		state.player.position.y = old_pos.y;
	}

	std::cout << "player collided: " << hasPlayerCollided(state) << '\n';
}

bool hasPlayerCollided(GameState& state) {

	float r = state.player.shape.getRadius();
	float px = state.player.position.x + r;
	float py = state.player.position.y + r;

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {

			auto tile = state.map.grid[i][j];
			
			if (tile == 0) continue;

			float x = j * TILE_SIZE;
			float y = i * TILE_SIZE;
			int w = TILE_SIZE, h = TILE_SIZE;
			
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