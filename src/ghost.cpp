#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
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
        sf::Vector2i ghost_tile = Utils::cartesianToTiledVector(ghost.position, Map::tilesize);

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

        DEBUG(
			"Ghost Position: Coordinate(%f, %f) Tiled(%i, %i)", 
			ghost.position.x, 
			ghost.position.y,
			(int)(ghost.position.x / Map::tilesize),
			(int)(ghost.position.y / Map::tilesize)
		);
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

        sf::Vector2i ghost_tile = Utils::cartesianToTiledVector(ghost.position, Map::tilesize);
        ghost.path = Ghost::findPath(ghost_tile, target_tile);

    }
    

    static inline int
    findTileNeighbours(sf::Vector2i tile, sf::Vector2i neighbours[]) {
    
        // find the 8 cardinal and valid neighbours of a tile

        int lower_limit_x = std::max(0, tile.x - 1);
        int lower_limit_y = std::max(0, tile.y - 1);
        int upper_limit_x = std::min(Map::width - 1, tile.x + 1);
        int upper_limit_y = std::min(Map::height - 1, tile.y + 1);
        
        int count = 0;
        
        for (int i = lower_limit_x; i <= upper_limit_x; i++) {
            for (int j = lower_limit_y; j <= upper_limit_y; j++) {
                if (i == tile.x && j == tile.y) continue;  
                if (Map::grid[j][i] != 0) continue;      
                neighbours[count++] = sf::Vector2i(i, j);
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


    // data structure needed for A*
    struct Node {
        sf::Vector2i pos;
        int* f_value;

        Node(sf::Vector2i _pos, int* _f): pos(_pos), f_value(_f) {};

        bool operator>(const Node& other) const {
            return *this->f_value > *other.f_value;
        }
    };

    using NodeHeap = std::priority_queue<Node, std::vector<Node>, std::greater<Node>>;

    // helper macro to treat the 1D vector as 2D matrix
    #define access(obj, a, b) obj[a * Map::height + b]

    std::vector<sf::Vector2i> 
    findPath(sf::Vector2i start, sf::Vector2i end) {

        /*
        Finds a path from start to end in Map::grid. Uses A* algorithm to achieve this.
        Reasons I used A*
        - It finds shortest path first and its faster than djikstra
        - It sounds cool
        */

        if (
            start.x < 0 || start.x >= Map::width || start.y < 0 || start.y >= Map::height 
            ||
            end.x < 0 || end.x >= Map::width || end.y < 0 || end.y >= Map::height
        ) {
            return {}; // out of bounds, abort pathfinding
        }

        if (start == end) return {}; // to not run the pathfinding algo when start == end

        // data structures needed for A*

        NodeHeap openheap; // stores the node with the least f_value at the top
        std::vector<std::byte> nodeset(Map::height * Map::width); // tracks whether a node is in closed(2), open(1) or none(0)
        std::vector<int> fvalueset(Map::height * Map::width); // tracks the f value of each node
        std::vector<sf::Vector2i> pathtrackset(Map::height * Map::width); // tracks the parent node of each node

        // optimization: i am using flat 1d vectors here instead of 2d arrays because
        // - vectors allocate on heap so the stack remains small (i encountered stack smashing when i used 2D arrays with large Map::grid)
        // - 1D vectors offer better cache locality as compared to 2D VECTORS

        // pushes the starting node to the open set
        openheap.push(Node(start, &access(fvalueset, start.y, start.x)));
        access(nodeset, start.y, start.x) = std::byte(1);

        // main loop, runs till there are nodes to examine
        while (!openheap.empty()) {

            // get the node with the least f value and move it to closed
            auto current = openheap.top();
            openheap.pop();
            access(nodeset, current.pos.y, current.pos.x) = std::byte(2);

            // BREAK condition 2, the loop exits if target is found
            if (current.pos == end) {
                break;
            }

            // finding the 8 (max) neighbours of our current node

            sf::Vector2i neighbours[8] = {};
            int neighbour_count = findTileNeighbours(current.pos, neighbours);

            for (int i = 0; i < neighbour_count; i++) {

                auto neighbour = neighbours[i];

                int y = neighbour.y;
                int x = neighbour.x;

                if (access(nodeset, y, x) == std::byte(2)) {
                    continue;    
                }

                // find the heuristics of our neighbour, g for distance from start, h for distance from end
                int g = findOctileDistance(neighbour, start);
                int h = findOctileDistance(neighbour, end);
                int f = g + h;

                // NOTE: the calculation of g here is not according to the standard A* implemenation
                // normal A* requires g to accumulated as we travel but here i compute it directly
                // primarily because its much simpler and the pathfinder still works as expected

                if (access(nodeset, y, x) != std::byte(1) || f < access(fvalueset, y, x)) {
                    // basically if the node is not in open OR the new path to node is quicker
                    access(fvalueset, y, x) = f;
                    access(pathtrackset, y, x) = current.pos;

                    access(nodeset, y, x) = std::byte(1);
                    openheap.push(Node(neighbour, &access(fvalueset, y, x)));
                }
            }
        }

        // backtracking to make the path

        std::vector<sf::Vector2i> path = {};

        auto current = end;
        while (true) {
            path.push_back(current);
            current = access(pathtrackset, current.y, current.x);
            if (current == start || current == sf::Vector2i(0,0)) break;
        }
        
        return path;
    }

    #undef access
}