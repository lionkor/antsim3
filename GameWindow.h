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

class Widget
    : public Object,
      public IEventReceiver
{
    OBJECT(Widget)
protected:
    Widget*           m_parent;
    class GameWindow* m_master;

public:
    Widget(Widget& parent);
    Widget(GameWindow& master);

    // IEventReceiver interface
public:
    virtual void handle(Event& e) override {
        report(__PRETTY_FUNCTION__ << " called with " << e);
    }

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

class FpsLogger
{
private:
    std::ofstream m_file;

public:
    FpsLogger(std::string filename) {
        m_file = std::ofstream(filename);
    }
    ~FpsLogger() {
        m_file << std::flush << std::endl;
        m_file.close();
    }

    void log_fps(double fps) {
        m_file << fps << "\n";
    }
};

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

    void zoom_view_at(sf::Vector2i pixel, float zoom);

public:
    typedef boost::shared_ptr<GameWindow> Pointer;

    static GameWindow::Pointer create(const std::string& title, sf::Vector2u size);

    void handle_events();
    void handle_mouse_button_press();
    void handle_mouse_button_release();
    void handle_key_press();
    void internal_draw();

    DrawSurface& surface() { return m_surface; }

    std::function<void(const vec<double>&)> on_left_click { nullptr };

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // GAMEWINDOW_H
