#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>
#include <cmath>
#include <chrono>

#include "Core.h"
#include "Utils.h"
#include "ECS/Entity.h"

/*

class SelectableObject : public PhysicalObject
{
    OBJECT(SelectableObject)
private:
    bool m_is_selected { false };

protected:
    virtual void draw(DrawSurface& surface) override {
        // recolor
        if (!m_has_index) {
            m_index     = surface.draw_new_rectangle(Rectangle(m_pos, m_size), Color::Green);
            m_has_index = true;
        }
        if (m_has_changed) {
            Color color;
            if (m_is_selected)
                color = Color::Magenta;
            else
                color = Color::Green;
            // FIXME: Why is this the way I chose to do it?!
            surface.update_rectangle(m_index, Rectangle(m_pos, m_size), color);
            m_has_changed = false;
        }
    }

public:
    SelectableObject(const vec<double> pos, const vec<double> size, World& world)
        : PhysicalObject(pos, size, world) { }

    virtual ~SelectableObject() override {
        m_world.reduce_selection(this);
    }

    // IHittable / PhysicalObject interface
public:
    virtual void on_hit(const vec<double>&) override {
        m_is_selected = !m_is_selected;
        m_has_changed = true;
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

class Particle
    : public PhysicalObject
{
    OBJECT(Particle)
private:
    vec<double> m_dir;
    double      m_speed;
    unsigned    m_max_life_ticks;
    unsigned    m_life_ticks { 0 };

protected:
    virtual void update() override {
        double perc = 1.0 - (double(m_life_ticks) / double(m_max_life_ticks));
        m_size *= std::sqrt(perc);
        m_pos += m_dir * m_speed;
        m_has_changed = true;
        ++m_life_ticks;

        if (m_life_ticks > m_max_life_ticks)
            destroy();
    }

public:
    Particle(vec<double> pos, unsigned lifetime_secs, World& world)
        : PhysicalObject(pos, { 3, 3 }, world)
        , m_dir(Random::random_real(-1.0, 1.0), Random::random_real(-1.0, 1.0))
        , m_speed(Random::random_real(0.01, 0.4))
        , m_max_life_ticks(lifetime_secs * 60) {
    }
};

class RandomlyMovingObject
    : public PhysicalObject
{
    OBJECT(RandomlyMovingObject)
private:
    vec<double> m_dir;
    double      m_speed;

protected:
    virtual void update() override {
        m_pos += m_dir * m_speed;
        m_has_changed = true;
    }

public:
    RandomlyMovingObject(const vec<double> pos, const vec<double> size, World& world)
        : PhysicalObject(pos, size, world)
        , m_dir(Random::random_real(-1.0, 1.0), Random::random_real(-1.0, 1.0))
        , m_speed(Random::random_real(0.01, 0.4)) { }

    // IHittable interface
public:
    virtual void on_hit(const vec<double>& hit_pos) override {
        destroy();
        int n = Random::random(10, 60);
        m_world.add_object(new RandomlyMovingObject({ 50, 50 }, { 10, 10 }, m_world));
        for (int i = 0; i < n; ++i) {
            m_world.add_object(new Particle(hit_pos, Random::random<unsigned>(1, 10), m_world));
        }
    }
};
*/

class MyComp1 : public Component
{
    OBJECT(MyComp1)
};

class MyComp1_1 : public MyComp1
{
    OBJECT(MyComp1_1)
};

class MyComp2 : public Component
{
    OBJECT(MyComp2)
};

class MyComp2_1 : public MyComp2
{
    OBJECT(MyComp2_1)
};

int main(int, char**) {

    Entity e;
    e.add_component<MyComp1_1>();
    e.add_component<MyComp2_1>();
    e.add_component<Component>();
    report("has component MyComp1: {}", e.has_component<MyComp1>());
    report("has component MyComp1_1: {}", e.has_component<MyComp1_1>());
    report("has component MyComp2: {}", e.has_component<MyComp2>());
    report("has component MyComp2_1: {}", e.has_component<MyComp2_1>());
    report("has component Component: {}", e.has_component<Component>());

    /*
    try {
        GameWindow::Pointer window = GameWindow::create("AntSim 3", sf::Vector2u { 1280, 720 });
        World::Pointer      world  = World::create();

        PhysicsObject obj({ 100, 200 }, { 40, 50 }, *world);

//        for (int i = 0; i < 1000; i += 10) {
  //          for (int k = 0; k < 1000; k += 10) {
    //            world->add_object(new SelectableObject(vec<double>(i, k), vec<double>(5, 5), *world));
      //      }
        //}

        for (int i = 0; i < 20; ++i) {
            world->add_object(new RandomlyMovingObject({ 50, 50 }, { 10, 10 }, *world));
        }

        window->on_left_click = [&](auto& pos) {
            world->try_hit(pos);
        };

        auto start = time(nullptr);
        while (window->isOpen()) {
            world->update(*window);
            if (CLOSE_AFTER != -1 && time(nullptr) > start + CLOSE_AFTER) {
                break;
            }
        }

    } catch (std::exception& e) {
        report_error("fatal exception occured: {}", e.what());
        throw e;
    }
*/
}
