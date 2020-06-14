#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <boost/container/vector.hpp>
#include <memory>

#include "Core/Object.h"
#include "Utils/DebugTools.h"
#include "Physics/vec.h"

struct Rectangle : public Object {
    OBJNAME(Rectangle)

    Rectangle(float x, float y, float w, float h)
        : pos(x, y), size(w, h) { }

    template<typename SomeT>
    Rectangle(SomeT _pos, SomeT _size)
        : pos(_pos), size(_size) { }

    vec<float> pos;
    vec<float> size;

    bool operator==(const Rectangle& rect) const { return pos == rect.pos && size == rect.size; }
    bool operator!=(const Rectangle& rect) const { return !(*this == rect); }

    // Object interface
public:
    virtual bool              operator==(const Object&) const override { return false; }
    virtual bool              operator!=(const Object&) const override { return true; }
    virtual std::stringstream to_stream() const override {
        auto ss = Object::to_stream();
        ss << "pos=" << pos << ";"
           << "size=" << size << ";";
        return ss;
    }
};

using Color = sf::Color;

class GameWindow;

class DrawSurface
{
    struct DrawRectangle {
        sf::Vertex vertices[4];
    };

    GameWindow&                             m_window;
    boost::container::vector<DrawRectangle> m_rects;
    boost::container::vector<std::size_t>   m_changed_indices;

    std::vector<sf::VertexArray> m_custom_varrays;
    std::vector<sf::Vertex>      m_vertices;

public:
    DrawSurface(GameWindow& window);

    void                      update_rectangle(size_t index, const Rectangle& rect, const Color& fill_color = Color::Green, const Color& border_color = Color::White);
    [[nodiscard]] std::size_t draw_new_rectangle(const Rectangle& rect, const Color& fill_color = Color::Green, const Color& border_color = Color::White);
    void                      remove_rectangle(size_t index);

    std::size_t submit_custom_varray(const sf::VertexArray& varray);
    void        update_custom_varray(size_t index, const sf::VertexArray& varray);

    void finalize();
};

#endif // DRAW_H
