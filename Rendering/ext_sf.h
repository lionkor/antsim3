#ifndef EXT_SF_H
#define EXT_SF_H

/*
 * Custom extensions for SFML
 */

#include "Physics/vec.h"
#include <SFML/Graphics.hpp>

class Color;

template<typename T>
concept Numeric = std::is_integral<T>::value || std::is_floating_point<T>::value;

namespace ext::sf {
inline ::sf::Vector2f to_sf_vec2f(const vec<float>& f) {
    return ::sf::Vector2f(f.x, f.y);
}
template<Numeric T>
inline ::sf::Vector2f to_sf_vec2f(T a, T b) {
    return ::sf::Vector2f(float(a), float(b));
}

::sf::Color to_sf_color(const Color& f);
}

#endif // EXT_SF_H
