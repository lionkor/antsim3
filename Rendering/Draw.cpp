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
    buf.vertices[0] = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y), fill_color);
    buf.vertices[1] = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), fill_color);
    buf.vertices[2] = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), fill_color);
    buf.vertices[3] = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), fill_color);
    std::size_t index = m_rects.size();
    m_rects.push_back(std::move(buf));
    m_changed_indices.push_back(index);
    return index;
}

void DrawSurface::finalize() {
    // if it takes too long, we could update until a certain amount of time has passed,
    // or just do this in a seperate thread
    for (const auto& index : m_changed_indices) {
        const sf::Vertex* verts = m_rects[index].vertices;
        m_window.draw(verts, 4, sf::PrimitiveType::Quads);
    }
    m_changed_indices.clear();
}
