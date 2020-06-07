#include "Application.h"

Application::Application(GameWindow*&& window, World*&& world)
    : m_window(std::move(window))
    , m_world(std::move(world)) {
}

int Application::run() {
    report("starting application with \n\tWorld: {}\n\tWindow: ", *m_world, *m_window);
    
    try {
    while (m_window->isOpen()) {
        m_world->update(*m_window);
    }
    } catch (std::exception& e) {
        report_error("fatal exception: {}", e.what());
        throw;
    }
    return 0;   
}
