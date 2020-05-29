#include "Application.h"

Application::Application(GameWindow*&& window, World*&& world)
    : m_window(std::move(window))
    , m_world(std::move(world)) {
}

int Application::run() {
    report("starting application with \n\tWorld: {}\n\tWindow: ", *m_world, *m_window);
    
    while (m_window->isOpen()) {
        m_world->update(*m_window);
    }
    return 0;   
}
