#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <boost/container/vector.hpp>
#include <memory>

#include "Object.h"
#include "DebugTools.h"
#include "vec.h"

struct Rectangle : public CopyableObject {
    OBJECT(Rectangle)

    Rectangle(double x, double y, double w, double h)
        : pos(x, y), size(w, h) { }

    template<typename SomeT>
    Rectangle(SomeT _pos, SomeT _size)
        : pos(_pos), size(_size) { }

    vec<double> pos;
    vec<double> size;

    bool operator==(const Rectangle& rect) const { return pos == rect.pos && size == rect.size; }
    bool operator!=(const Rectangle& rect) const { return !(*this == rect); }

    // Object interface
public:
    virtual bool              operator==(const Object&) const override { return false; }
    virtual bool              operator!=(const Object&) const override { return true; }
    virtual std::stringstream to_stream() const override {
        auto ss = CopyableObject::to_stream();
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

    GameWindow&                          m_window;
    boost::container::vector<sf::Vertex> m_verts;
    std::unique_ptr<sf::VertexBuffer>    m_vb;
    bool                                 m_changed { true };

public:
    DrawSurface(GameWindow& window);

    void                      update_rectangle(std::size_t index, const Rectangle& rect, const Color& fill_color = Color::Green, const Color& border_color = Color::White);
    [[nodiscard]] std::size_t draw_new_rectangle(const Rectangle& rect, const Color& fill_color = Color::Green, const Color& border_color = Color::White);

    void finalize();
};

#endif // DRAW_H
