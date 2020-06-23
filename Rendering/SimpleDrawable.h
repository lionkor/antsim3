// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef SIMPLEDRAWABLE_H
#define SIMPLEDRAWABLE_H

#include "Rendering/Draw.h"

class SimpleDrawable
    : public Object
{
    OBJNAME(SimpleDrawable)
private:
    mutable bool    m_initialized { false };
    mutable size_t  m_index {};
    mutable bool    m_changed { true };
    sf::VertexArray m_varray {};
    sf::Texture*    m_texture { nullptr };

public:
    typedef sf::PrimitiveType PrimitiveType;
    typedef sf::Vertex        Vertex;
    typedef sf::Vector2f      Vector2f;

    SimpleDrawable() = default;
    SimpleDrawable(PrimitiveType primitive, size_t size)
        : m_varray(primitive, size) { }

    void          set_texture(sf::Texture* texture);
    void          set_primitive(PrimitiveType primitive) { m_varray.setPrimitiveType(primitive); }
    Vertex&       operator[](size_t index) { return m_varray[index]; }
    const Vertex& operator[](size_t index) const { return m_varray[index]; }
    void          resize(size_t size);
    void          clear();
    void          draw(DrawSurface& surface) const;
    size_t        size() const { return m_varray.getVertexCount(); }
    void          set_changed() { m_changed = true; }
    bool          has_changed() const { return m_changed; }
};

#endif // SIMPLEDRAWABLE_H
