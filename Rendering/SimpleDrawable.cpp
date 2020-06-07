#include "SimpleDrawable.h"

void SimpleDrawable::draw(DrawSurface& surface) const {
    if (!m_initialized) {
        m_index       = surface.submit_custom_varray(m_varray);
        m_initialized = true;
    }
    surface.update_custom_varray(m_index, m_varray);
}
