#include "World.h"
#include "Utils/DebugTools.h"
#include "Utils/Random.h"
#include "Physics/Ray.h"

#include <algorithm>

void World::cleanup_destroyed(DrawSurface& surface) {
    decltype(m_entities)::iterator iter_to_erase;
    do {
        iter_to_erase = std::find_if(m_entities.begin(), m_entities.end(),
            [](SharedPtr<Entity>& entity_ptr) -> bool {
                return entity_ptr->is_marked_destroyed();
            });
        if (iter_to_erase != m_entities.end()) {
            (*iter_to_erase)->on_cleanup(surface);
            report("destroying {}", **iter_to_erase);
            m_entities.erase(iter_to_erase);
        }
    } while (iter_to_erase != m_entities.end());
}

void World::add_new_entities() {
    for (auto& entity : m_entities_to_add) {
        m_entities.push_back(std::move(entity));
    }
    m_entities_to_add.clear();
}

World::World() {
    m_update_timer.restart();
    m_entities_to_add.reserve(1000);
    m_entities.reserve(1000);
}

WeakPtr<Entity> World::add_entity(Entity*&& obj) {
    ASSERT(obj != nullptr);
    m_entities_to_add.push_back(SharedPtr<Entity>(std::move(obj)));
    obj = nullptr;
    auto entity            = WeakPtr<Entity>(m_entities_to_add.back());
    entity.lock()->m_world = this;
    return entity;
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
    if (!m_application)
        throw std::runtime_error("no application set, this can't be right!");
    
    add_new_entities();

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

    cleanup_destroyed(surface);
}

std::stringstream World::to_stream() const {
    return Object::to_stream();
}
