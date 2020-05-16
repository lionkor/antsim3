#include "World.h"
#include "DebugTools.h"
#include "Random.h"
#include "Ray.h"

World::World() {
}

void World::add_object(PhysicalObject* obj) {
    m_objects.emplace_back(std::unique_ptr<PhysicalObject>(obj));
}

RayHit&& World::try_hit(const vec<double>& pos) {
    Ray ray(pos);
    for (auto& object : m_objects) {
        ray.try_intersect(*object);
    }
    RayHit hit = ray.result();
    for (auto& h : hit) {
        h->on_hit(pos);
    }
    return std::move(hit);
}

void World::extend_selection(const PhysicalObject* ptr) {
    report("extended by: " << *ptr);
    m_selected_objects.insert(ptr);
}

void World::reduce_selection(const PhysicalObject* ptr) {
    report("reduced by: " << *ptr);
    m_selected_objects.erase(ptr);
}

void World::update(GameWindow& window) {
    window.clear();
    window.internal_draw();

    auto& surface = window.surface();
    for (auto& obj : m_objects) {
        obj->draw(surface);
    }
    surface.finalize();

    window.display();
    window.handle_events();
}

std::stringstream World::to_stream() const {
    return Object::to_stream();
}

