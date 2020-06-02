#include "World.h"
#include "Utils/DebugTools.h"
#include "Utils/Random.h"
#include "Physics/Ray.h"

#include <algorithm>

World::World() {
    m_update_timer.restart();
}

Entity& World::add_entity(Entity*&& obj) {
    ASSERT(obj != nullptr);
    m_entities.push_back(Managed<Entity>(std::move(obj)));
    return *m_entities.back();
}

RayHit World::try_hit(const vec<double>& pos) {
    Ray ray(pos);
    for (auto& object : m_entities) {
        // TODO BROKEN MESS
    }
    RayHit hit = ray.result();
    for (auto& h : hit) {
        h->on_hit(pos);
    }
    return hit;
}

void World::update(GameWindow& window) {
    window.clear();
    window.internal_draw();

    // FIXME: This might be slow
    auto ms = std::size_t(m_update_timer.getElapsedTime().asMilliseconds());
    if (m_update_interval_ms == 0 || ms >= m_update_interval_ms) {
        for (auto& entity : m_entities) {
            entity->on_update();
        }
        m_update_timer.restart();
    }

    auto& surface = window.surface();
    for (auto& entity : m_entities) {
        entity->on_draw(surface);
    }
    surface.finalize();

    window.display();
    window.handle_events();
}

std::stringstream World::to_stream() const {
    return Object::to_stream();
}
