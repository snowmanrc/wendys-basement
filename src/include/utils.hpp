#pragma once

#include <SFML/System/Vector2.hpp>
#include <algorithm>

namespace Utils {

    static inline bool 
    detectCircleRectCollision(float cr, float cx, float cy, float rx, float ry, float rw, float rh) {
        
        float closest_x = std::max(rx, std::min(cx, rx + rw));
        float closest_y = std::max(ry, std::min(cy, ry + rh));

        float dist_x = cx - closest_x;
        float dist_y = cy - closest_y;
        float dist_sq = (dist_x * dist_x) + (dist_y * dist_y);

        // optimization: not taking sqrt() because its an expensive operation

        return dist_sq <= (cr * cr);
    }

    static inline sf::Vector2i
    cartesianToTiledVector(sf::Vector2f v, int scale) {
        return {
        static_cast<int>(v.x / scale), 
        static_cast<int>(v.y / scale)
        };
    }
}
