#include "Drawable.h"

#include "Core/GameWindow.h"

#include "Utils/Random.h"

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
    m_shape[1] = sf::Vertex(ext::sf::to_sf_vec2f(m_pos) + sf::Vector2f(m_size.x, 0), { m_color.r, m_color.g, m_color.b, m_color.a }, sf::Vector2f(tex_coords.x - 1, 0.0f));
    m_shape[2] = sf::Vertex(ext::sf::to_sf_vec2f(m_pos) + sf::Vector2f(m_size.x, m_size.y), { m_color.r, m_color.g, m_color.b, m_color.a }, tex_coords + sf::Vector2f(-1, -1));
    m_shape[3] = sf::Vertex(ext::sf::to_sf_vec2f(m_pos) + sf::Vector2f(0, m_size.y), { m_color.r, m_color.g, m_color.b, m_color.a }, sf::Vector2f(0.0f, tex_coords.y - 1));
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
    update_internal_shape();
}

void Rectangle::draw(GameWindow& window) const {
    if (m_texture) {
        window.draw(m_shape, sf::RenderStates(m_texture));
    } else {
        window.draw(m_shape);
    }
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

bool DrawablePointerWrapper::operator==(const DrawablePointerWrapper& wrapper) const {
    return ptr->id() == wrapper.ptr->id();
}

static constexpr size_t index_3to1(size_t x, size_t y, size_t z, size_t width, size_t depth) {
    return x * depth + y * width * depth + z;
}

TileMap::TileMap(vec<size_t> grid_size, double tile_size, SharedPtr<TextureAtlas> atlas)
    : m_varray(sf::PrimitiveType::Quads, grid_size.x * grid_size.y * 4) // x * y * (4 vertices)
    , m_grid_size(grid_size)
    , m_tile_size(tile_size)
    , m_atlas(atlas) {
    ASSERT(m_atlas.get() != nullptr);
    auto [top_left, bottom_right] = m_atlas->subtexture_coords({ 0, 0 });
    for (size_t x = 0; x < grid_size.x; ++x) {
        for (size_t y = 0; y < grid_size.y; ++y) {
            m_varray[index_3to1(x, y, 0, m_grid_size.x, 4)].position = { float(x * m_tile_size), float(y * m_tile_size) };
            m_varray[index_3to1(x, y, 1, m_grid_size.x, 4)].position = { float(x * m_tile_size + m_tile_size), float(y * m_tile_size) };
            m_varray[index_3to1(x, y, 2, m_grid_size.x, 4)].position = { float(x * m_tile_size + m_tile_size), float(y * m_tile_size + m_tile_size) };
            m_varray[index_3to1(x, y, 3, m_grid_size.x, 4)].position = { float(x * m_tile_size), float(y * m_tile_size + m_tile_size) };
            m_varray[index_3to1(x, y, 0, m_grid_size.x, 4)].color = sf::Color::White;
            m_varray[index_3to1(x, y, 1, m_grid_size.x, 4)].color = sf::Color::White;
            m_varray[index_3to1(x, y, 2, m_grid_size.x, 4)].color = sf::Color::White;
            m_varray[index_3to1(x, y, 3, m_grid_size.x, 4)].color = sf::Color::White;
            m_varray[index_3to1(x, y, 0, m_grid_size.x, 4)].texCoords = sf::Vector2f { top_left.x, top_left.y };
            m_varray[index_3to1(x, y, 1, m_grid_size.x, 4)].texCoords = sf::Vector2f { bottom_right.x, top_left.y };
            m_varray[index_3to1(x, y, 2, m_grid_size.x, 4)].texCoords = sf::Vector2f { bottom_right.x, bottom_right.y };
            m_varray[index_3to1(x, y, 3, m_grid_size.x, 4)].texCoords = sf::Vector2f { top_left.x, bottom_right.y };
        }
    }
}

void TileMap::set_tile_color(vec<size_t> tile_index, Color color) {
    m_varray[index_3to1(tile_index.x, tile_index.y, 0, m_grid_size.x, 4)].color = ext::sf::to_sf_color(color);
    m_varray[index_3to1(tile_index.x, tile_index.y, 1, m_grid_size.x, 4)].color = ext::sf::to_sf_color(color);
    m_varray[index_3to1(tile_index.x, tile_index.y, 2, m_grid_size.x, 4)].color = ext::sf::to_sf_color(color);
    m_varray[index_3to1(tile_index.x, tile_index.y, 3, m_grid_size.x, 4)].color = ext::sf::to_sf_color(color);
}

void TileMap::set_tile_texture(vec<size_t> tile_index, vec<size_t> atlas_index) {
    auto [top_left, bottom_right] = m_atlas->subtexture_coords(atlas_index);
    m_varray[index_3to1(tile_index.x, tile_index.y, 0, m_grid_size.x, 4)].texCoords = sf::Vector2f { top_left.x, top_left.y };
    m_varray[index_3to1(tile_index.x, tile_index.y, 1, m_grid_size.x, 4)].texCoords = sf::Vector2f { bottom_right.x, top_left.y };
    m_varray[index_3to1(tile_index.x, tile_index.y, 2, m_grid_size.x, 4)].texCoords = sf::Vector2f { bottom_right.x, bottom_right.y };
    m_varray[index_3to1(tile_index.x, tile_index.y, 3, m_grid_size.x, 4)].texCoords = sf::Vector2f { top_left.x, bottom_right.y };
}

void TileMap::randomize_textures() {
    auto random_index = [&]() -> vec<size_t> {
        return { Random::random<size_t>(0, atlas_size().x - 1), Random::random<size_t>(0, atlas_size().y - 1) };
    };
    for (size_t x = 0; x < m_grid_size.x; ++x) {
        for (size_t y = 0; y < m_grid_size.y; ++y) {
            set_tile_texture({ x, y }, random_index());
        }
    }
}

void TileMap::draw(GameWindow& window) const {
    window.draw(m_varray, sf::RenderStates(sf::BlendAlpha, sf::Transform().translate(m_position.x, m_position.y), m_atlas->texture(), nullptr));
}
