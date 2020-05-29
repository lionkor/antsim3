#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <fstream>

#include "IEventReceiver.h"
#include "DebugTools.h"
#include "Object.h"
#include "Ray.h"
#include "Draw.h"

class FpsLogger
{
private:
    std::ofstream m_file;

public:
    FpsLogger(const std::string& filename) {
        m_file = std::ofstream(filename);
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
      public boost::enable_shared_from_this<GameWindow>,
      public sf::RenderWindow,
      public EventDispatcher
{
    OBJECT(GameWindow)
protected:
    GameWindow(const std::string& title, sf::Vector2u size);

    sf::Event    m_event;
    sf::Vector2i m_mouse_pos;
    bool         m_mmb_pressed { false };
    DrawSurface  m_surface;
    FpsLogger    m_fps_logger;
    sf::Clock    m_fps_clock;
    std::string       m_title;

    /// Zooms the current camera's view by zoom, moving relative to the specified 
    /// pixel.
    void zoom_view_at(sf::Vector2i pixel, float zoom);

public:
    typedef boost::shared_ptr<GameWindow> Pointer;

    /// Creates a new GameWindow, returns a pointer. This is the proper way of 
    /// creating a window.
    static GameWindow::Pointer create(const std::string& title, sf::Vector2u size);

    void handle_events();
    void handle_mouse_button_press();
    void handle_mouse_button_release();
    void handle_key_press();
    void internal_draw();
    
    /// Gets the title.
    const std::string& title() const;
    /// Sets the title property and calls into sfml to change the title.
    void set_title(const std::string& title);

    /// Gets the surface to be used for drawing geometry into the window.
    DrawSurface& surface() { return m_surface; }

    /// Callback that is called whenever the left mouse button has been clicked.
    std::function<void(const vec<double>&)> on_left_click { nullptr };

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // GAMEWINDOW_H