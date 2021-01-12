#ifndef EXT_SF_H
#define EXT_SF_H

/*
 * Custom extensions for SFML
 */

#include <SFML/System.hpp>
#include "Physics/vec.h"

namespace ext::sf {
static inline ::sf::Vector2f to_sf_vec2f(const vec<float>& f) {
    return ::sf::Vector2f(f.x, f.y);
}
}

#endif // EXT_SF_H
