#include "Drawable.h"

#include "Core/GameWindow.h"

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
    m_shape.setSize({ float(m_size.x), float(m_size.y) });
    m_shape.setPosition(float(m_pos.x), float(m_pos.y));
    m_shape.setRotation(float(m_rotation));
    m_shape.setFillColor({ m_color.r, m_color.g, m_color.b, m_color.a });
    m_shape.setScale(float(m_scale), float(m_scale));
    if (m_texture) {
        m_shape.setTexture(m_texture);
        m_shape.setTextureRect(sf::IntRect(0, 0, m_texture->getSize().x, m_texture->getSize().y));
    }
}

void Rectangle::set_texture(sf::Texture* texture) {
    ASSERT(texture);
    m_texture = texture;
}

void Rectangle::draw(GameWindow& window) {
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
