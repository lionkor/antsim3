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

public:
    GuiLayer(Application& app);

    template<typename T, typename... Args>
    requires(std::derived_from<T, Widget>)
        [[nodiscard]] WeakPtr<T> add_widget(Args&&... args);

    void on_update(float);
    void on_draw(DrawSurface& surface);

    Application& application() { return m_app; }

    virtual void on_mouse_click(const HID::MouseAction& ma);
};

class Widget : public Object {
private:
    GuiLayer& m_gui;
    vecd m_pos;
    vecd m_size;
    Color m_background_color { Color::White };
    Color m_foreground_color { Color::Black };
    sf::Text m_text;

public:
    Widget(GuiLayer& layer, const vecd& pos, const vecd& size);

    GuiLayer& gui_layer() { return m_gui; }
    const vecd& position() const { return m_pos; }
    const vecd& size() const { return m_size; }
    const Color& foreground_color() const { return m_foreground_color; }
    const Color& background_color() const { return m_background_color; }
    std::string text() const { return m_text.getString(); }

    void set_position(const vecd& pos) { m_pos = pos; }
    void set_size(const vecd& size) { m_size = size; }
    void set_foreground_color(const Color& color) { m_foreground_color = color; }
    void set_background_color(const Color& color) { m_background_color = color; }
    void set_text(const std::string& str) { m_text.setString(str); }

    virtual void on_update(float);
    // in Widget::on_draw, the view is reset, so drawing happens screen-space
    virtual void on_draw(DrawSurface& surface);

    std::function<void(Widget&, const vecd&)> on_click_callback { nullptr };
};

template<typename T, typename... Args>
requires(std::derived_from<T, Widget>)
    [[nodiscard]] WeakPtr<T> GuiLayer::add_widget(Args&&... args) {
    SharedPtr<T> ptr(new T(*this, std::forward<Args>(args)...));
    m_widgets.push_back(ptr);
    return ptr;
}

#endif // GUILAYER_H
