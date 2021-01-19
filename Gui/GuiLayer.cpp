#include "GuiLayer.h"
#include "Core/Application.h"

GuiLayer::GuiLayer(Application& app)
    : m_app(app) {
}

void GuiLayer::on_update(float dt) {
    if (!m_enabled) {
        return;
    }
    for (auto& widget : m_widgets) {
        if (!widget->is_enabled()) {
            continue;
        }
        widget->on_update(dt);
    }
}

void GuiLayer::on_draw(GameWindow& surface) {
    if (!m_enabled) {
        return;
    }
    for (auto& widget : m_widgets) {
        if (!widget->is_enabled()) {
            continue;
        }
        widget->on_draw(surface);
    }
}

void GuiLayer::on_mouse_click(const HID::MouseAction& ma) {
    if (!m_enabled) {
        return;
    }
    for (auto& widget : m_widgets) {
        if (!widget->is_enabled()) {
            continue;
        }
        if (ma.button == HID::MouseButton::Left
            && ma.screen_position.x >= widget->position().x
            && ma.screen_position.x <= widget->position().x + widget->size().x
            && ma.screen_position.y >= widget->position().y
            && ma.screen_position.y <= widget->position().y + widget->size().y) {
            // widget clicked
            if (widget->on_click_callback) {
                widget->on_click_callback(*widget, vecd(ma.screen_position));
            }
        }
    }
}

Widget::Widget(GuiLayer& layer, const vecd& pos, const vecd& size, const std::string& text, const sf::Font& font)
    : m_gui(layer)
    , m_pos(pos)
    , m_size(size)
    , m_text(text)
    , m_font(font)
    , m_rect(m_pos, m_size)
    , m_drawable_text(m_pos, 20, text, font) {
    m_rect.set_disable_fn([](Drawable&) {});
    m_drawable_text.set_disable_fn([](Drawable&) {});
}

void Widget::on_update(float) {
    m_drawable_text.set_position(m_pos);
    m_drawable_text.set_color(m_foreground_color);
    m_drawable_text.set_text(m_text);
    if (m_resize_to_text) {
        auto extents = m_drawable_text.extents();
        m_rect.set_size(extents + vecd(5, 5));
    } else {
        m_rect.set_size(m_size);
    }
    m_size = m_rect.size();
    m_rect.set_position(m_pos);
    m_rect.set_color(m_background_color);
}

void Widget::on_draw(GameWindow& surface) {
    m_rect.draw(surface);
    m_drawable_text.draw(surface);
}

std::stringstream Widget::to_stream() const {
    TS_BEGIN(Object);
    TS_PROP_M_STR(m_text);
    TS_PROP_M(m_pos);
    TS_PROP_M(m_size);
    TS_PROP_M(m_foreground_color);
    TS_PROP_M(m_background_color);
    TS_END();
}
