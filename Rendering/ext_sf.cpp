#include "ext_sf.h"

#include "Rendering/Drawable.h"

sf::Color ext::sf::to_sf_color(const Color& f) {
    return ::sf::Color(f.r, f.g, f.b, f.a);
}
