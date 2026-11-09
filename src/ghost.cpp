#include <SFML/System/Vector2.hpp>
#include <functional>
#include <queue>
#include <vector>

#include "include/ghost.hpp"
#include "include/debug.hpp"
#include "include/map.hpp"
#include "include/utils.hpp"


namespace Ghost {


    void updatePosition(Entity& ghost, float dt) {
        
        if (ghost.path.empty()) return;

        auto target_tile = ghost.path.back();
        sf::Vector2i ghost_tile = {
            static_cast<int>((ghost.position.x) / Map::tilesize), 
            static_cast<int>((ghost.position.y) / Map::tilesize)
        };

        if (target_tile == ghost_tile) {
            ghost.path.pop_back();
            return;
        }

        auto dir_vec = target_tile - ghost_tile;
        sf::Vector2f dir_vecf = {(float)dir_vec.x, (float)dir_vec.y}; 
        auto normalized_vec = dir_vecf.normalized();

        auto old_pos = ghost.position;

		ghost.position.x += normalized_vec.x * ghost.speed * dt;
		if (hasCollided(ghost)) {
			ghost.position.x = old_pos.x;
		}

		ghost.position.y += normalized_vec.y * ghost.speed * dt;
		if (hasCollided(ghost)) {
			ghost.position.y = old_pos.y;
		}
    }


    bool hasCollided(Entity& ghost) {
        
		float cr = ghost.shape.getRadius();
		float cx = ghost.position.x + cr;
		float cy = ghost.position.y + cr;

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

    void follow(Entity& ghost, sf::Vector2i target_tile) {

        sf::Vector2i ghost_tile = {
            static_cast<int>((ghost.position.x + ghost.size) / Map::tilesize), 
            static_cast<int>((ghost.position.y + ghost.size) / Map::tilesize)
        };

        ghost.path = Ghost::findPath(ghost_tile, target_tile);
    }
    

    struct Node {
        sf::Vector2i pos;
        int* f_value;

        bool operator>(const Node& other) const {
            return *this->f_value > *other.f_value;
        }
    };


    using NodeHeap = std::priority_queue<Node, std::vector<Node>, std::greater<Node>>;


    static inline int

    findTileNeighbours(sf::Vector2i tile, sf::Vector2i neighbours[]) {
    
        int lower_limit_x = std::max(0, tile.x - 1);
        int lower_limit_y = std::max(0, tile.y - 1);
        int upper_limit_x = std::min(Map::width - 1, tile.x + 1);
        int upper_limit_y = std::min(Map::height - 1, tile.y + 1);
        
        int count = 0;
        
        for (int i = lower_limit_x; i <= upper_limit_x; i++) {
            for (int j = lower_limit_y; j <= upper_limit_y; j++) {
                if (i == tile.x && j == tile.y) continue;  
                if (Map::grid[j][i] != 0) continue;      
                neighbours[count++] = {i, j};
            }
        }
        
        return count;
        
    };
    

    static inline int
    findOctileDistance(sf::Vector2i a, sf::Vector2i b) {
        int dx = std::abs(a.x - b.x);
        int dy = std::abs(a.y - b.y);
        // formula for octile distance, note that it has been scaled by a factor of 10 so we do not have to deal with floats
        return (14 - 10) * std::min(dx, dy) + 10 * std::max(dx, dy); 
    }


    std::vector<sf::Vector2i> 
    findPath(sf::Vector2i start, sf::Vector2i end) {

        /*
        Finds a path from start to end in Map::grid. Uses A* algorithm to achieve this.
        Reasons I used A*
        - It finds shortest path first and its faster than djikstra
        - It sounds cool
        */

        if (start.x < 0 || start.x >= Map::width || start.y < 0 || start.y >= Map::height ||
            end.x < 0 || end.x >= Map::width || end.y < 0 || end.y >= Map::height) {
            return {}; // Out of bounds, abort pathfinding
        }

        if (start == end) return {};

        NodeHeap openheap;
        std::byte nodeset[Map::height][Map::width] = {};
        int fvalueset[Map::height][Map::width] = {};
        sf::Vector2i pathtrackset[Map::height][Map::width] = {};

        openheap.push((Node) {start, &fvalueset[start.y][start.x]});

        while (!openheap.empty()) {

            auto min_node = openheap.top();
            openheap.pop();
            nodeset[min_node.pos.y][min_node.pos.x]= std::byte(2);

            if (min_node.pos == end) {
                break;
            }

            sf::Vector2i neighbours[8] = {};
            int neighbour_count = findTileNeighbours(min_node.pos, neighbours);

            for (int i = 0; i < neighbour_count; i++) {

                auto neighbour = neighbours[i];

                if (nodeset[neighbour.y][neighbour.x] == std::byte(2)) {
                    continue;    
                }

                int g = findOctileDistance(neighbour, start);
                int h = findOctileDistance(neighbour, end);
                int f = g + h;

                if (nodeset[neighbour.y][neighbour.x] != std::byte(1) || f < fvalueset[neighbour.y][neighbour.x]) {

                    fvalueset[neighbour.y][neighbour.x] = f;
                    pathtrackset[neighbour.y][neighbour.x] = min_node.pos;

                    nodeset[neighbour.y][neighbour.x] = std::byte(1);
                    openheap.push({.pos = neighbour, .f_value = &fvalueset[neighbour.y][neighbour.x]});
                }
            }
        }

        std::vector<sf::Vector2i> path = {};

        auto current = end;
        while (true) {
            path.push_back(current);
            current = pathtrackset[current.y][current.x];
            if (current == start || current == (sf::Vector2i) {0,0}) break;
        }
        
        return path;
    }
}