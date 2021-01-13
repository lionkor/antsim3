#include "Drawable.h"

#include "Core/GameWindow.h"

#include "ext_sf.h"

const Color Color::Red(255, 0, 0, 255);
const Color Color::Green(0, 255, 0, 255);
const Color Color::Blue(0, 0, 255, 255);
const Color Color::White(255, 255, 255, 255);
const Color Color::Black(0, 0, 0, 255);
const Color Color::Transparent(0, 0, 0, 0);

static std::atomic_size_t s_id_counter;

static Drawable::ID generate_new_id() {
    return { ++s_id_counter };
}

void Rectangle::update_internal_shape() {
    sf::Vector2f tex_coords { 0, 0 };
    if (m_texture) {
        tex_coords = sf::Vector2f(m_texture->getSize());
    }
    m_shape[0] = sf::Vertex(ext::sf::to_sf_vec2f(m_pos), { m_color.r, m_color.g, m_color.b, m_color.a }, { 0.0f, 0.0f });
    m_shape[1] = sf::Vertex(ext::sf::to_sf_vec2f(m_pos) + sf::Vector2f(m_size.x, 0), { m_color.r, m_color.g, m_color.b, m_color.a }, sf::Vector2f(tex_coords.x, 0.0f));
    m_shape[2] = sf::Vertex(ext::sf::to_sf_vec2f(m_pos) + sf::Vector2f(m_size.x, m_size.y), { m_color.r, m_color.g, m_color.b, m_color.a }, tex_coords);
    m_shape[3] = sf::Vertex(ext::sf::to_sf_vec2f(m_pos) + sf::Vector2f(0, m_size.y), { m_color.r, m_color.g, m_color.b, m_color.a }, sf::Vector2f(0.0f, tex_coords.y));
}

Rectangle::Rectangle(vecd pos, vecd size, double rotation)
    : m_pos(pos)
    , m_size(size)
    , m_rotation(rotation)
    , m_shape(sf::PrimitiveType::Quads, 4) {
    update_internal_shape();
}

void Rectangle::set_texture(sf::Texture* texture) {
    ASSERT(texture);
    m_texture = texture;
}

void Rectangle::draw(GameWindow& window) const {
    report("drawing in Rectangle");
    window.draw(m_shape);
}

Drawable::Drawable()
    : m_id(generate_new_id()) {
}

Drawable::Drawable(const Drawable&)
    : m_id(generate_new_id()) {
}

Drawable& Drawable::operator=(const Drawable&) {
    m_id = generate_new_id();
    return *this;
}
