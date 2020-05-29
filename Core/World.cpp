#include "World.h"
#include "Utils/DebugTools.h"
#include "Utils/Random.h"
#include "Physics/Ray.h"

#include <algorithm>

World::World() {
}

World::Pointer World::create() {
    return Pointer(new World());
}

void World::add_object(PhysicalObject* obj) {
    m_objects.emplace_back(std::unique_ptr<PhysicalObject>(obj));
}

RayHit World::try_hit(const vec<double>& pos) {
    Ray ray(pos);
    for (auto& object : m_objects) {
        ray.try_intersect(*object);
    }
    RayHit hit = ray.result();
    for (auto& h : hit) {
        h->on_hit(pos);
    }
    return hit;
}

void World::extend_selection(const PhysicalObject* ptr) {
    m_selected_objects.insert(ptr);
}

void World::reduce_selection(const PhysicalObject* ptr) {
    m_selected_objects.erase(ptr);
}

void World::update(GameWindow& window) {
    window.clear();
    window.internal_draw();
    
    // FIXME: This might be slow
    for (auto& obj : m_objects) {
        obj->try_update();
    }
    
    const auto find_destroyed_pred = [&](const auto& obj) -> bool {
        return obj->destroyed();
    };
    
    auto iter = std::find_if(m_objects.begin(), m_objects.end(), find_destroyed_pred);
    while (iter != m_objects.end()) {
        m_objects.erase(iter);
        iter = std::find_if(m_objects.begin(), m_objects.end(), find_destroyed_pred);
    }
    
    auto& surface = window.surface();
    for (auto& obj : m_objects) {
        obj->try_draw(surface);
    }
    surface.finalize();

    window.display();
    window.handle_events();
}

std::stringstream World::to_stream() const {
    return Object::to_stream();
}
