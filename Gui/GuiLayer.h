#ifndef GUILAYER_H
#define GUILAYER_H

#include "Core/HID.h"
#include "Core/Object.h"
#include "Rendering/Draw.h"
#include "Utils/Managed.h"

#include <vector>

class Widget;

class GuiLayer : public Object {
    OBJNAME(GuiLayer)
private:
    class Application& m_app;
    std::vector<SharedPtr<Widget>> m_widgets;
    bool m_enabled { true };

public:
    GuiLayer(Application& app);

    template<typename T, typename... Args>
    requires(std::derived_from<T, Widget>)
        [[nodiscard]] WeakPtr<T> add_widget(Args&&... args);

    void on_update(float);
    void on_draw(GameWindow& surface);

    Application& application() { return m_app; }

    virtual void on_mouse_click(const HID::MouseAction& ma);
};

class Widget : public Object {
    OBJNAME(Widget)
private:
    GuiLayer& m_gui;
    vecd m_pos;
    vecd m_size;
    Color m_background_color { Color::White };
    Color m_foreground_color { Color::Black };
    std::string m_text;
    sf::Font m_font;
    Rectangle m_rect;
    Text m_drawable_text;
    bool m_resize_to_text { false };
    bool m_enabled { true };

public:
    Widget(GuiLayer& layer, const vecd& pos, const vecd& size, const std::string& text, const sf::Font& font);
    Widget(const Widget&) = delete;

    GuiLayer& gui_layer() { return m_gui; }
    const vecd& position() const { return m_pos; }
    const vecd& size() const { return m_size; }
    const Color& foreground_color() const { return m_foreground_color; }
    const Color& background_color() const { return m_background_color; }
    std::string text() const { return m_text; }
    void enable() { m_enabled = true; }
    void disable() { m_enabled = false; }
    bool is_enabled() const { return m_enabled; }

    void set_position(const vecd& pos) { m_pos = pos; }
    void set_size(const vecd& size) { m_size = size; }
    void set_foreground_color(const Color& color) { m_foreground_color = color; }
    void set_background_color(const Color& color) { m_background_color = color; }
    void set_text(const std::string& str) { m_text = str; }
    void set_resize_to_text(bool resize) { m_resize_to_text = resize; }

    virtual void on_update(float);
    // in Widget::on_draw, the view is reset, so drawing happens screen-space
    virtual void on_draw(GameWindow& surface);

    std::function<void(Widget&, const vecd&)> on_click_callback { nullptr };

    virtual std::stringstream to_stream() const override;
};

template<typename T, typename... Args>
requires(std::derived_from<T, Widget>)
    [[nodiscard]] WeakPtr<T> GuiLayer::add_widget(Args&&... args) {
    SharedPtr<T> ptr(new T(*this, std::forward<Args>(args)...));
    m_widgets.push_back(ptr);
    return ptr;
}

#endif // GUILAYER_H
