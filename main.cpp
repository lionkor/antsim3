#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>

#include "DebugTools.h"
#include "GameWindow.h"
#include "World.h"

class SelectableObject : public PhysicalObject
{
    OBJECT(SelectableObject)
private:
    bool m_is_selected { false };

public:
    SelectableObject(const vec<double> pos, const vec<double> size, World& world)
        : PhysicalObject(pos, size, world) { }

    virtual ~SelectableObject() {
        m_world.reduce_selection(this);
    }

    virtual void draw(DrawSurface& surface) override {
        // recolor
        if (!m_has_index) {
            m_index     = surface.draw_new_rectangle(Rectangle(m_pos, m_size), Color::Green);
            m_has_index = true;
        }
        if (m_changed) {
            Color color;
            if (m_is_selected)
                color = Color::Magenta;
            else
                color = Color::Green;
            // FIXME: Why is this the way I chose to do it?!
            surface.update_rectangle(m_index, Rectangle(m_pos, m_size), color);
            m_changed = false;
        }
    }

    // IHittable / PhysicalObject interface
public:
    virtual void on_hit(const vec<double>&) override {
        m_is_selected = !m_is_selected;
        m_changed     = true;
        if (m_is_selected)
            m_world.extend_selection(this);
        else
            m_world.reduce_selection(this);
    }

    virtual std::stringstream to_stream() const override {
        auto ss = PhysicalObject::to_stream();
        ss << "is_selected=" << m_is_selected << ";";
        return ss;
    }
};

int main(int, char**) {
    try {
        GameWindow::Pointer window;
        {
            window = GameWindow::create("AntSim 3", sf::Vector2u { 1280, 720 });
        }
        World::Pointer world = World::create();

        for (int i = 0; i < 1000; i += 10) {
            for (int k = 0; k < 1000; k += 10) {
                world->add_object(new SelectableObject(vec<double>(i, k), vec<double>(10, 10), *world));
            }
        }

        window->on_left_click = [&](auto& pos) {
            world->try_hit(pos);
        };

        while (window->isOpen()) {
            world->update(*window);
            report_error(window->title());
        }

    } catch (std::exception& e) {
        report_error("fatal exception occured: ", e.what());
    }
}
