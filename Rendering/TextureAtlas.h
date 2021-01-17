#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "Core/Object.h"
#include "Utils/Managed.h"
#include "Physics/vec.h"
#include <SFML/Graphics/Texture.hpp>

class TextureAtlas final : public Object
{
    OBJNAME(TextureAtlas)
private:
    Managed<sf::Texture> m_atlas;
    size_t m_subtexture_size;

public:
    TextureAtlas(Managed<sf::Texture> atlas, size_t subtexture_size);

    std::pair<vec<float> /* top left */, vec<float> /* bottom right */> subtexture_coords(vec<size_t> subtexture_index) const;

    sf::Texture* texture() { return m_atlas.get(); }

    vec<size_t> atlas_size() const {
        auto size = m_atlas->getSize();
        return vec<size_t>(size.x / m_subtexture_size, size.y / m_subtexture_size);
    }
};

#endif // TEXTUREATLAS_H
