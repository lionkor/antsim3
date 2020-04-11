#include "GameWindow.h"
#include "DebugTools.h"

GameWindow::GameWindow(const std::string& title, sf::Vector2u size)
    : sf::RenderWindow(sf::VideoMode(size.x, size.y), title)
    , m_main_widget(*this) {
    setFramerateLimit(60);
}

void GameWindow::zoom_view_at(sf::Vector2i pixel, float zoom) {
    const sf::Vector2f beforeCoord { mapPixelToCoords(pixel) };
    sf::View           view { getView() };
    view.zoom(zoom);
    setView(view);
    const sf::Vector2f afterCoord { mapPixelToCoords(pixel) };
    const sf::Vector2f offsetCoords { beforeCoord - afterCoord };
    view.move(offsetCoords);
    setView(view);
}

GameWindow::Pointer GameWindow::create(const std::string& title, sf::Vector2u size) {
    return Pointer(new GameWindow(title, size));
}

void GameWindow::handle_events() {
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
                view.move(-sf::Vector2f(diff));
                setView(view);
            }
            m_mouse_pos = sf::Mouse::getPosition(*this);
            break;
        }
        case sf::Event::MouseWheelScrolled: {
            if (m_event.mouseWheelScroll.delta > 0)
                zoom_view_at({ m_event.mouseWheelScroll.x, m_event.mouseWheelScroll.y }, (1.f / 1.1f));
            else if (m_event.mouseWheelScroll.delta < 0)
                zoom_view_at({ m_event.mouseWheelScroll.x, m_event.mouseWheelScroll.y }, 1.1f);
        }
        default:
            report_trace("not handled: " << m_event.type);
        }
    }
}

void GameWindow::handle_mouse_button_press() {
    if (m_event.mouseButton.button == sf::Mouse::Middle) {
        m_mmb_pressed = true;
    } else if (m_event.mouseButton.button == sf::Mouse::Left) {
        Event e;
        report("dispatching event " << &e);
        m_dispatcher.dispatch(e);
    }
}

void GameWindow::handle_mouse_button_release() {
    if (m_event.mouseButton.button == sf::Mouse::Middle) {
        m_mmb_pressed = false;
    }
}

void GameWindow::handle_key_press() {
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

void GameWindow::internal_draw() {}

EventDispatcher& GameWindow::dispatcher() {
    return m_dispatcher;
}

std::stringstream GameWindow::to_stream() const {
    auto ss = Object::to_stream();
    auto size = getSize();
    ss << "width=" << size.x << ";";
    ss << "height=" << size.y << ";";
    return ss;
}

Widget::Widget(Widget& parent)
    : IEventReceiver(parent.m_master->dispatcher())
    , m_parent(&parent)
    , m_master(parent.m_master) {
    report(__PRETTY_FUNCTION__ << " called");
}

Widget::Widget(GameWindow& master)
    : IEventReceiver(master.dispatcher())
    , m_parent(nullptr)
    , m_master(&master) {
    report(__PRETTY_FUNCTION__ << " called");
}

std::stringstream Widget::to_stream() const {
    auto ss = Object::to_stream();
    if (m_parent)
        ss << "parent=" << *m_parent << ";";
    else 
        ss << "parent=nullptr;";
    if (m_master) 
        ss << "master=" << *m_master << ";";
    else 
        ss << "master=nullptr;";
    return ss;
}
