#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <chrono>
#include <concepts>
#include <fstream>
#include <memory>

#include <limits>

#include "Core/Object.h"
#include "Gui/GuiLayer.h"
#include "Physics/Ray.h"
#include "Rendering/Draw.h"
#include "Rendering/GuiElement.h"
#include "Utils/Managed.h"

class FpsLogger {
private:
    std::ofstream m_file;

public:
    explicit FpsLogger(const std::string& filename)
        : m_file(std::ofstream(filename)) {
    }
    ~FpsLogger() {
        m_file << std::flush << std::endl;
        m_file.close();
    }

    /// Creates an entry in the fps logger's file.
    void log_fps(double fps) {
        m_file << fps << "\n";
    }
};

/// Manages anything window- and rendering related.
class GameWindow
    : public Object,
      public sf::RenderWindow {
    OBJNAME(GameWindow)

    friend class Application;

protected:
    sf::Event m_event;
    sf::Vector2i m_mouse_pos;
    bool m_mmb_pressed { false };
    DrawSurface m_surface;
    FpsLogger m_fps_logger;
    sf::Clock m_fps_clock;
    std::string m_title;
    sf::Color m_clear_color;
    class Application& m_application;
    std::vector<SharedPtr<GuiLayer>> m_gui_layers; // should be very few

    // FIXME: We should check somewhere if this is not nullptr anymore

    /// Zooms the current camera's view by zoom, moving relative to the specified
    /// pixel.
    void zoom_view_at(sf::Vector2i pixel, float zoom);

public:
    GameWindow(Application& app, const std::string& title, sf::Vector2u size, bool fullscreen = false);

    void handle_events();
    void handle_mouse_button_press();
    void handle_mouse_button_release();
    void handle_key_press();
    void handle_key_release();
    void internal_draw();

    /// Gets the title.
    const std::string& title() const { return m_title; }
    /// Sets the title property and calls into sfml to change the title.
    void set_title(const std::string& title);

    /// Gets the surface to be used for drawing geometry into the window.
    DrawSurface& surface() { return m_surface; }

    void set_framerate_limit(std::size_t limit) { setFramerateLimit(static_cast<unsigned int>(limit)); }

    template<typename T>
    T width() const { return static_cast<T>(getSize().x); }

    template<typename T>
    T height() const { return static_cast<T>(getSize().y); }

    veci mouse_screen_pos() const { return veci(m_mouse_pos); }
    vecd mouse_world_pos() const { return vecd(mapPixelToCoords(m_mouse_pos)); }

    /// Callback that is called whenever the left mouse button has been clicked.
    std::function<void(const vecd&)> on_left_click { nullptr };

    template<typename T, typename... Args>
    requires(std::derived_from<T, GuiLayer>)
        [[nodiscard]] WeakPtr<T> add_gui_layer(Args&&...);

    const std::vector<SharedPtr<GuiLayer>>& gui_layers() const {
        return m_gui_layers;
    }

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

template<typename T, typename... Args>
requires(std::derived_from<T, GuiLayer>)
    [[nodiscard]] WeakPtr<T> GameWindow::add_gui_layer(Args&&... args) {
    SharedPtr<T> ptr(new T(m_application, std::forward<Args>(args)...));
    m_gui_layers.push_back(ptr);
    return ptr;
}

#endif // GAMEWINDOW_H
