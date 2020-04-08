#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>
#include "DebugTools.h"

class Tool
{
protected:
    class GameWindow* m_window;

public:
    Tool(class GameWindow* window)
        : m_window(window) {}
    virtual ~Tool() = default;
    Tool(Tool&&)    = default;
    Tool& operator=(Tool&&) = default;

    Tool(const Tool&) = delete;
    Tool& operator=(const Tool&) = delete;

    virtual void draw() {}

    virtual void handle_mouse_button_press(sf::Mouse::Button, sf::Vector2i pos)   = 0;
    virtual void handle_mouse_button_release(sf::Mouse::Button, sf::Vector2i pos) = 0;
};

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom) {
    const sf::Vector2f beforeCoord { window.mapPixelToCoords(pixel) };
    sf::View           view { window.getView() };
    view.zoom(zoom);
    window.setView(view);
    const sf::Vector2f afterCoord { window.mapPixelToCoords(pixel) };
    const sf::Vector2f offsetCoords { beforeCoord - afterCoord };
    view.move(offsetCoords);
    window.setView(view);
}

class GameWindow
    : public sf::RenderWindow,
      public boost::enable_shared_from_this<GameWindow>
{
protected:
    GameWindow(const std::string& title, sf::Vector2u size);

    sf::Event    m_event;
    sf::Vector2i m_mouse_pos;
    bool         m_mmb_pressed { false };

public:
    typedef boost::shared_ptr<GameWindow> Pointer;

    static GameWindow::Pointer create(const std::string& title, sf::Vector2u size) {
        return Pointer(new GameWindow(title, size));
    }
    virtual ~GameWindow() override {}

    void handle_events() {
        while (pollEvent(m_event)) {
            switch (m_event.type) {
            case sf::Event::Closed:
                report_trace(nameof(sf::Event::Closed));
                close();
                break;
            case sf::Event::KeyPressed:
                report_trace(nameof(sf::Event::KeyPressed) << " with key.code=" << m_event.key.code);
                handle_key_press();
                break;
            case sf::Event::MouseButtonPressed:
                handle_mouse_button_press();
                break;
            case sf::Event::MouseButtonReleased:
                handle_mouse_button_release();
                break;
            case sf::Event::MouseMoved: {
                if (m_mmb_pressed) {
                    auto diff = mapPixelToCoords(sf::Mouse::getPosition(*this)) - mapPixelToCoords(m_mouse_pos);
                    auto view = getView();
                    view.move(- sf::Vector2f(diff));
                    setView(view);
                }
                m_mouse_pos = sf::Mouse::getPosition(*this);
                break;
            }
            case sf::Event::MouseWheelScrolled: {
                if (m_event.mouseWheelScroll.delta > 0)
                    zoomViewAt({ m_event.mouseWheelScroll.x, m_event.mouseWheelScroll.y }, *this, (1.f / 1.1f));
                else if (m_event.mouseWheelScroll.delta < 0)
                    zoomViewAt({ m_event.mouseWheelScroll.x, m_event.mouseWheelScroll.y }, *this, 1.1f);
            }
            default:
                report_trace("not handled: " << m_event.type);
            }
        }
    }

    void handle_mouse_button_press() {
        if (m_event.mouseButton.button == sf::Mouse::Middle) {
            m_mmb_pressed = true;
        }
    }

    void handle_mouse_button_release() {
        if (m_event.mouseButton.button == sf::Mouse::Middle) {
            m_mmb_pressed = false;
        }
    }

    void handle_key_press() {
        ASSERT(m_event.type == sf::Event::KeyPressed);
        switch (m_event.key.code) {
        case sf::Keyboard::Escape:
            report("closing through Escape");
            close();
            break;
        default:
            report_trace("not handled: " << m_event.key.code);
        }
    }

    void internal_draw() {}

    // Window interface
protected:
    virtual void onResize() override {
        sf::RenderWindow::onResize();
    }
};

int main(int, char**) {
    try {
        GameWindow::Pointer window = GameWindow::create("AntSim 3", sf::Vector2u { 1280, 720 });

        while (window->isOpen()) {
            window->clear();
            window->draw(sf::RectangleShape({ 200, 100 }), sf::Transform().translate(10, 10));
            window->internal_draw();
            window->display();
            window->handle_events();
        }
    } catch (std::exception& e) {
        report_error("exception occured: " << e.what());
    }
}

GameWindow::GameWindow(const std::string& title, sf::Vector2u size)
    : sf::RenderWindow(sf::VideoMode(size.x, size.y), title) {
    setFramerateLimit(60);
}
