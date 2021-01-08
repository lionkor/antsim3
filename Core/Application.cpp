#include "Application.h"

Application::Application(const std::string& title, sf::Vector2u size, bool fullscreen, const std::string& res_file_path)
    : m_window(new GameWindow(*this, title, size, fullscreen))
    , m_world(new World(*this))
    , m_resource_manager(res_file_path) {
}

int Application::run() {
    report("starting application with \n\tWorld: {}\n\tWindow: ", *m_world, *m_window);

    try {
        sf::Clock dt_clock;
        while (m_window->isOpen()) {
            m_world->update(*m_window, dt_clock.restart().asSeconds());
        }
    } catch (const std::exception& e) {
        report_error("fatal exception: {}", e.what());
        throw;
    }
    return 0;
}

std::stringstream Application::to_stream() const {
    TS_BEGIN(Object);
    TS_PROP_S("window", *m_window);
    TS_PROP_S("world", *m_world);
    TS_END();
}
