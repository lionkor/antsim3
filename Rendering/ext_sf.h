#ifndef EXT_SF_H
#define EXT_SF_H

/*
 * Custom extensions for SFML
 */

#include <SFML/Graphics.hpp>
#include "Physics/vec.h"

class Color;

namespace ext::sf {
static inline ::sf::Vector2f to_sf_vec2f(const vec<float>& f) {
    return ::sf::Vector2f(f.x, f.y);
}
::sf::Color to_sf_color(const Color& f);
}

#endif // EXT_SF_H
