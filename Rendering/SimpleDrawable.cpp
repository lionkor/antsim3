#include "SimpleDrawable.h"

void SimpleDrawable::set_texture(sf::Texture* texture) {
    m_texture = texture;
}

void SimpleDrawable::resize(size_t size) {
    m_varray.resize(size);
    m_changed = true;
}

void SimpleDrawable::clear() {
    m_varray.clear();
    m_changed = true;
}

void SimpleDrawable::draw(DrawSurface& surface) const {
    if (!m_initialized) {
        m_index       = surface.submit_custom_varray(m_varray, m_texture);
        m_initialized = true;
    }
    if (m_changed) {
        surface.update_custom_varray(m_index, m_varray, m_texture);
        m_changed = false;
    }
}
