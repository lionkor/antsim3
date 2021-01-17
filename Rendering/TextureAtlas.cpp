#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(Managed<sf::Texture> atlas, size_t subtexture_size)
    : m_atlas(std::move(atlas))
    , m_subtexture_size(subtexture_size) {
    atlas = nullptr;
    ASSERT(m_atlas.get() != nullptr);
    auto size = m_atlas->getSize();
    if (size.x % m_subtexture_size != 0 || size.y % m_subtexture_size != 0) {
        report_error("the size of the texture atlas base texture ({}, {}) is not a multiple of the subtexture size ({}, {})", size.x, size.y, m_subtexture_size, m_subtexture_size);
    }
}

std::pair<vec<float>, vec<float> > TextureAtlas::subtexture_coords(vec<size_t> subtexture_index) const {
    return {
        vec<float>(subtexture_index.x * m_subtexture_size, subtexture_index.y * m_subtexture_size),
        vec<float>((subtexture_index.x + 1) * m_subtexture_size, (subtexture_index.y + 1) * m_subtexture_size)
    };
}
