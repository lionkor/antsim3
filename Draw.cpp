#include "Draw.h"
#include "GameWindow.h"

DrawSurface::DrawSurface(GameWindow& window)
    : m_window(window) {
    m_verts.reserve(50000);
    m_vb = std::make_unique<sf::VertexBuffer>(sf::PrimitiveType::Quads, sf::VertexBuffer::Usage::Dynamic);
}

void DrawSurface::update_rectangle(std::size_t index, const Rectangle& rect, const Color& fill_color, const Color&) {
    // FIXME: Check what changed and change only that
    m_verts.at(index + 0) = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y), fill_color);
    m_verts.at(index + 1) = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), fill_color);
    m_verts.at(index + 2) = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), fill_color);
    m_verts.at(index + 3) = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), fill_color);
    m_changed             = true;
}

std::size_t DrawSurface::draw_new_rectangle(const Rectangle& rect, const Color& fill_color, const Color&) {
    DrawRectangle dr;
    dr.vertices[0] = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y), fill_color);
    dr.vertices[1] = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), fill_color);
    dr.vertices[2] = sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), fill_color);
    dr.vertices[3] = sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), fill_color);
    m_verts.push_back(dr.vertices[0]);
    auto index = m_verts.size() - 1;
    m_verts.push_back(dr.vertices[1]);
    m_verts.push_back(dr.vertices[2]);
    m_verts.push_back(dr.vertices[3]);
    m_changed = true;
    return index;
}

void DrawSurface::finalize() {
    if (m_changed) {
        report_trace("update");
        m_vb->create(m_verts.size());
        m_vb->update(reinterpret_cast<sf::Vertex*>(m_verts.data()));
        m_changed = false;
    }
    m_window.draw(*m_vb);
}
