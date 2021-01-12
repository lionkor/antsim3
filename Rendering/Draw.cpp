#include "Draw.h"
#include "Core/GameWindow.h"

DrawSurface::DrawSurface(GameWindow& window)
    : m_window(window) {
    m_changed_indices.reserve(50'000);
    m_rects.reserve(50'000);
    m_gui_view = m_window.getView();
}

void DrawSurface::draw(const Drawable& drawable) {
    m_drawables.push(&drawable);
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
    auto cached_view = m_window.getView();
    m_window.setView(m_gui_view);
    for (auto& text : m_texts) {
        m_window.draw(text.get());
    }
    m_window.setView(cached_view);
    m_texts.clear();
}
