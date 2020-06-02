#include "GameWindow.h"
#include "Utils/DebugTools.h"

GameWindow::GameWindow(const std::string& title, sf::Vector2u size)
    : sf::RenderWindow(sf::VideoMode(size.x, size.y), title)
    , m_surface(*this)
    , m_fps_logger("fps.csv")
    , m_title(title) {
    setFramerateLimit(120);
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

void GameWindow::handle_events() {
    while (pollEvent(m_event)) {
        switch (m_event.type) {
        case sf::Event::Closed:
            report_trace(nameof(sf::Event::Closed));
            close();
            break;
        case sf::Event::KeyPressed:
            report_trace(nameof(sf::Event::KeyPressed), " with key.code=", m_event.key.code);
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
            break;
        }
        default:
            report_trace("not handled: ", m_event.type);
        }
    }
}

void GameWindow::handle_mouse_button_press() {
    if (m_event.mouseButton.button == sf::Mouse::Middle) {
        m_mmb_pressed = true;
    } else if (m_event.mouseButton.button == sf::Mouse::Left) {
        vec<int>    screen_pos(m_event.mouseButton);
        vec<double> world_pos = mapPixelToCoords({ screen_pos.x, screen_pos.y });
        if (on_left_click)
            on_left_click(world_pos);
        Event e;
        dispatch(e);
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
        report_trace("not handled: ", m_event.key.code);
    }
}

void GameWindow::internal_draw() {
    auto time = m_fps_clock.restart();
    auto val  = 1.0 / time.asSeconds();
    if (val < 140)
        m_fps_logger.log_fps(1.0 / time.asSeconds());
}

const std::string& GameWindow::title() const {
    return m_title;
}

void GameWindow::set_title(const std::string& title) {
    m_title = title;
    setTitle(title);
}

void GameWindow::set_framerate_limit(std::size_t limit) {
    setFramerateLimit(limit);
}

std::stringstream GameWindow::to_stream() const {
    auto ss   = Object::to_stream();
    auto size = getSize();
    ss << "width=" << size.x << ";";
    ss << "height=" << size.y << ";";
    return ss;
}
