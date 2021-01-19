#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Core/Object.h"
#include "Physics/vec.h"
#include "TextureAtlas.h"

#include <SFML/Graphics.hpp>
#include <functional>

struct Color final {
    uint8_t r, g, b, a;
    Color() = default;
    Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
        : r(_r)
        , g(_g)
        , b(_b)
        , a(_a) {
    }
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color White;
    static const Color Black;
    static const Color Transparent;
};

// pointer to Drawable, with operator== which compares by Drawable::ID
struct DrawablePointerWrapper {
    const class Drawable* ptr;
    bool operator==(DrawablePointerWrapper wrapper) const;
};

class Drawable : public Object {
    OBJNAME(Drawable)
public:
    struct ID {
        size_t value;
        auto operator<=>(const ID& rhs) const = default;
    };

protected:
    ID m_id;
    // called on destruction, to be used to call a function that removes
    // this object from a draw-stack.
    mutable std::function<void(Drawable&)> m_disable_fn { nullptr };

public:
    Drawable();
    Drawable(const Drawable&);
    Drawable& operator=(const Drawable&);

    virtual ~Drawable() {
        if (m_disable_fn) {
            m_disable_fn(*this);
        } else {
            report_warning("drawable with ID {} was not given a disable_fn. this might crash the game.", m_id.value);
        }
    }

    virtual void set_position(const vecd& new_pos) = 0;
    virtual void set_rotation(double new_rot) = 0;
    virtual void set_color(Color color) = 0;
    virtual void set_scale(double) = 0;
    virtual vecd position() const = 0;
    virtual double rotation() const = 0;
    virtual Color color() const = 0;
    virtual double scale() const = 0;

    virtual void draw(class GameWindow&) const = 0;

    //  final
    virtual ID id() const final { return m_id; }
    virtual DrawablePointerWrapper get_pointer() const final { return { this }; }
    virtual void set_disable_fn(decltype(m_disable_fn) disable_fn) const final { m_disable_fn = disable_fn; }
};

namespace std {
template<>
struct hash<DrawablePointerWrapper> {
    std::size_t operator()(DrawablePointerWrapper s) const noexcept {
        return s.ptr->id().value;
    }
};
}

class Rectangle : public Drawable {
    OBJNAME(Rectangle)
private:
    vecd m_pos;
    vecd m_size;
    double m_scale { 1.0 };
    Color m_color;
    double m_rotation;
    sf::VertexArray m_shape;
    sf::Texture* m_texture { nullptr };

    // to be called whenever a field changes
    void update_internal_shape();

public:
    Rectangle(const vecd& pos = { 0, 0 }, const vecd& size = { 10, 10 }, double rotation = 0);

    virtual void set_position(const vecd& new_pos) override {
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

    virtual void draw(class GameWindow& window) const override;
};

class TileMap : public Drawable {
    OBJNAME(TileMap)
private:
    sf::VertexArray m_varray;
    vec<size_t> m_grid_size;
    double m_tile_size;
    vecd m_position;
    SharedPtr<TextureAtlas> m_atlas;

public:
    TileMap(const vec<size_t>& grid_size, double tile_size, SharedPtr<TextureAtlas> atlas);

    virtual void set_position(const vecd& new_pos) override { m_position = new_pos; }
    virtual void set_rotation(double) override { NOTIMPL; }
    virtual void set_color(Color) override { NOTIMPL; }
    virtual void set_scale(double) override { NOTIMPL; }
    virtual vecd position() const override { return m_position; }
    virtual double rotation() const override { return 0; }
    virtual Color color() const override { return Color::Black; }
    virtual double scale() const override { return 1; }

    vec<size_t> grid_size() const { return m_grid_size; }

    [[deprecated]] void set_tile_color(const vec<size_t>& tile_index, Color color);
    void set_tile_texture(const vec<size_t>& tile_index, const vec<size_t>& atlas_index);

    vec<size_t> atlas_size() const {
        return m_atlas->atlas_size();
    }

    void randomize_textures();

    virtual void draw(GameWindow&) const override;
};

class Text : public Drawable {
    OBJNAME(Text)
private:
    vecd m_position;
    double m_font_size;
    sf::Font m_font;
    sf::Text m_text;

public:
    Text(const vecd& pos, uint32_t font_size, const std::string& text, const sf::Font& font);

    void set_position(const vecd& new_pos) { m_position = new_pos; }
    void set_rotation(double new_rot) {}
    void set_color(Color color);
    void set_scale(double);
    vecd position() const;
    double rotation() const;
    Color color() const;
    double scale() const;
    void draw(GameWindow&) const;
};

#endif // DRAWABLE_H
