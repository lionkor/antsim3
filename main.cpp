#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>

#include "DebugTools.h"
#include "GameWindow.h"
#include "World.h"


int main(int, char**) {
    try {
        GameWindow::Pointer window = GameWindow::create("AntSim 3", sf::Vector2u { 1280, 720 });
        World               world(10, 10);

        while (window->isOpen()) {
            window->clear();
            world.tick();
            window->draw(sf::RectangleShape({ 200, 100 }), sf::Transform().translate(10, 10));
            window->internal_draw();
            window->display();
            window->handle_events();
        }
    } catch (std::exception& e) {
        report_error("fatal exception occured: " << e.what());
    }
}
