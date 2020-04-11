#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "IEventReceiver.h"
#include "DebugTools.h"
#include "Object.h"

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
    virtual void handle(Event&) override {
        report(__PRETTY_FUNCTION__ << " CALLED!!!");
    }
    
    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

class GameWindow
    : public Object,
      public boost::enable_shared_from_this<GameWindow>,
      public sf::RenderWindow
{
    OBJECT(GameWindow)
protected:
    GameWindow(const std::string& title, sf::Vector2u size);

    sf::Event       m_event;
    sf::Vector2i    m_mouse_pos;
    bool            m_mmb_pressed { false };
    Widget          m_main_widget;
    EventDispatcher m_dispatcher;

    void zoom_view_at(sf::Vector2i pixel, float zoom);

public:
    virtual ~GameWindow() {}

    typedef boost::shared_ptr<GameWindow> Pointer;

    static GameWindow::Pointer create(const std::string& title, sf::Vector2u size);

    void             handle_events();
    void             handle_mouse_button_press();
    void             handle_mouse_button_release();
    void             handle_key_press();
    void             internal_draw();
    EventDispatcher& dispatcher();
    
    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // GAMEWINDOW_H
