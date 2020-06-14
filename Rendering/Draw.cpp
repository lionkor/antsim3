#include "Draw.h"
#include "Core/GameWindow.h"

DrawSurface::DrawSurface(GameWindow& window)
    : m_window(window) {
    m_changed_indices.reserve(50'000);
    m_rects.reserve(50'000);
}

void DrawSurface::update_rectangle(std::size_t index, const Rectangle& rect, const Color& fill_color, const Color&) {
    // FIXME: Check what changed and change only that
    m_rects[index].vertices[0] = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y), fill_color);
    m_rects[index].vertices[1] = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), fill_color);
    m_rects[index].vertices[2] = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), fill_color);
    m_rects[index].vertices[3] = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), fill_color);
    m_changed_indices.push_back(index);
}

std::size_t DrawSurface::draw_new_rectangle(const Rectangle& rect, const Color& fill_color, const Color&) {
    DrawRectangle buf;
    buf.vertices[0]   = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y), fill_color);
    buf.vertices[1]   = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), fill_color);
    buf.vertices[2]   = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), fill_color);
    buf.vertices[3]   = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), fill_color);
    std::size_t index = m_rects.size();
    m_rects.push_back(std::move(buf));
    m_changed_indices.push_back(index);
    return index;
}

void DrawSurface::remove_rectangle(size_t index) {
    m_rects.erase(m_rects.begin() + index);
     m_changed_indices.push_back(index);
}

std::size_t DrawSurface::submit_custom_varray(const sf::VertexArray& varray, sf::Texture* texture) {
    m_custom_varrays.push_back({ varray, texture });
    return m_custom_varrays.size() - 1;
}

void DrawSurface::update_custom_varray(std::size_t index, const sf::VertexArray& varray, sf::Texture* texture) {
    m_custom_varrays.at(index) = { varray, texture };
}

void DrawSurface::finalize() {
    // if it takes too long, we could update until a certain amount of time has passed,
    // or just do this in a seperate thread
    if (m_changed_indices.size() != 0) {
        m_vertices.clear();
        for (const auto& rect : m_rects) {
            const sf::Vertex* verts = rect.vertices;
            m_vertices.insert(m_vertices.end(), verts, verts + 4);
        }
        m_changed_indices.clear();
    }
    m_window.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::Quads);
    // FIXME: This sucks but we need something like this.
    for (auto& varray : m_custom_varrays) {
        m_window.draw(varray.varray, sf::RenderStates(varray.texture));
    }
}
