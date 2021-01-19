#include "GuiLayer.h"
#include "Core/Application.h"

GuiLayer::GuiLayer(Application& app)
    : m_app(app) {
}

void GuiLayer::on_update(float dt) {
    for (auto& widget : m_widgets) {
        widget->on_update(dt);
    }
}

void GuiLayer::on_draw(DrawSurface& surface) {
    // reset view to defaults
    auto old_view = m_app.window().getView();
    m_app.window().setView(sf::View());

    for (auto& widget : m_widgets) {
        widget->on_draw(surface);
    }

    // set view to previous view
    m_app.window().setView(old_view);
}

void GuiLayer::on_mouse_click(const HID::MouseAction& ma) {
    for (auto& widget : m_widgets) {
        if (ma.button == HID::MouseButton::Left
            && ma.screen_position.x >= widget->position().x
            && ma.screen_position.x < widget->position().x + widget->size().x
            && ma.screen_position.y >= widget->position().y
            && ma.screen_position.y < widget->position().y + widget->size().y) {
            // widget clicked
            if (widget->on_click_callback) {
                widget->on_click_callback(*widget, vecd(ma.screen_position));
            }
        }
    }
}

void Widget::on_update(float) {
}

void Widget::on_draw(DrawSurface& surface) {
    Rectangle rect(m_pos, m_size);
    // TODO: implement foreground color, etc.
    rect.set_color(m_background_color);
    surface.draw(rect);

}
