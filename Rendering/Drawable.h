#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Physics/vec.h"
#include "Core/Object.h"

#include <SFML/Graphics.hpp>

struct Color final {
    uint8_t r, g, b, a;
    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : r(r), g(g), b(b), a(a) {
    }
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color White;
    static const Color Black;
    static const Color Transparent;
};

class Drawable : public Object
{
    OBJNAME(Drawable)
public:
    struct ID {
        size_t value;
        bool operator==(const ID& id) {
            return value == id.value;
        }
    };

protected:
    ID m_id;

public:
    Drawable();
    Drawable(const Drawable&);
    Drawable& operator=(const Drawable&);

    virtual ~Drawable() = default;

    virtual void set_position(vecd new_pos) = 0;
    virtual void set_rotation(double new_rot) = 0;
    virtual void set_color(Color color) = 0;
    virtual void set_scale(double) = 0;

    virtual vecd position() const = 0;
    virtual double rotation() const = 0;
    virtual Color color() const = 0;
    virtual double scale() const = 0;

    virtual void draw(class GameWindow&) = 0;

    virtual const ID& id() const final { return m_id; }
};

class Rectangle : public Drawable
{
    OBJNAME(Rectangle)
private:
    vecd m_pos;
    vecd m_size;
    double m_scale;
    Color m_color;
    double m_rotation;
    sf::RectangleShape m_shape;
    sf::Texture* m_texture { nullptr };

    // to be called whenever a field changes
    void update_internal_shape();

public:
    Rectangle(vecd pos, vecd size, double rotation = 0)
        : m_pos(pos)
        , m_size(size)
        , m_rotation(rotation) {
        update_internal_shape();
    }

    virtual void set_position(vecd new_pos) override {
        m_pos = new_pos;
        update_internal_shape();
    }
    virtual void set_rotation(double new_rot) override {
        m_rotation = new_rot;
        update_internal_shape();
    }
    virtual void set_color(Color color) override {
        m_color = color;
        update_internal_shape();
    }
    virtual void set_scale(double scale) override {
        m_scale = scale;
        update_internal_shape();
    }
    virtual vecd position() const override { return m_pos; }
    virtual double rotation() const override { return m_rotation; }
    virtual Color color() const override { return m_color; }
    virtual double scale() const override { return m_scale; }

    vecd size() const { return m_size; }
    void set_size(vecd size) { m_size = size; }

    void set_texture(sf::Texture* texture);

    virtual void draw(class GameWindow& window) override;
};

#endif // DRAWABLE_H
