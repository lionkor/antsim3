#include "Draw.h"
#include "Core/GameWindow.h"

DrawSurface::DrawSurface(GameWindow& window)
    : m_window(window) {
    // what the fuck does this do?!
    m_gui_view = m_window.getView();
}

void DrawSurface::draw(const Drawable& drawable) {
    m_drawables.insert(drawable.get_pointer());
    drawable.set_disable_fn([this](const Drawable& _drawable) {
        report("drawable released");
        m_drawables.erase(_drawable.get_pointer());
    });
}

void DrawSurface::finalize() {
    for (const auto& drawable : m_drawables) {
        drawable.ptr->draw(m_window);
    }

    // if it takes too long, we could update until a certain amount of time has passed,
    // or just do this in a seperate thread
    auto cached_view = m_window.getView();
    m_gui_view.reset(sf::FloatRect(0, 0, m_window.getSize().x, m_window.getSize().y));
    m_window.setView(m_gui_view);
    for (auto& layer : m_window.gui_layers()) {
        layer->on_draw(m_window);
    }
    m_window.setView(cached_view);

    m_window.display();
}
