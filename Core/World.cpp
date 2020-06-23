// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "World.h"
#include "Utils/DebugTools.h"
#include "Utils/Random.h"
#include "Physics/Ray.h"
#include "Application.h"
#include "ECS/Entity.h"

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

World::World(Application& app)
    : m_application(app) {
    m_update_timer.restart();
    m_entities_to_add.reserve(1000);
    m_entities.reserve(1000);
}

WeakPtr<Entity> World::add_entity(const vecd& pos) {
    m_entities_to_add.push_back(std::make_shared<Entity>(*this, pos));
    report_function();
    auto entity = WeakPtr<Entity>(m_entities_to_add.back());
    return entity;
}

WeakPtr<Entity> World::add_entity(const Entity& entity) {
    m_entities_to_add.push_back(make_shared<Entity>(entity));
    auto e = WeakPtr<Entity>(m_entities_to_add.back());
    return e;
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
    auto& surface = window.surface();

    add_new_entities();

    window.clear(surface.clear_color());
    window.internal_draw();

    // FIXME: This might be slow
    auto ms = std::size_t(m_update_timer.getElapsedTime().asMilliseconds());
    if (m_update_interval_ms == 0 || ms >= m_update_interval_ms) {
        for (auto& entity : m_entities) {
            entity->on_update();
        }
        m_update_timer.restart();
    }

    for (auto& entity : m_entities) {
        entity->on_draw(surface);
    }

    auto& gui_elems = m_application.gui_elements();
    for (auto& element : gui_elems) {
        element->update();
        element->draw(surface);
    }
    
    surface.finalize();
    
    window.display();
    window.handle_events();

    cleanup_destroyed(surface);
}

std::stringstream World::to_stream() const {
    TS_BEGIN(Object);
    TS_PROP_M(m_entities.size());
    TS_PROP_M(m_entities_to_add.size());
    TS_PROP_M(m_update_interval_ms);
    TS_PROP_M(m_application.uuid());
    TS_END();
}
