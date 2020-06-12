#include "Entity.h"

void Entity::on_cleanup(DrawSurface& surface) {
    for (auto& comp : m_comps) {
        comp->on_cleanup(surface);
    }
}

Entity::Entity(const vec<double>& pos)
    // all entitie have a transform component
    : m_transform(add_component(new TransformComponent(pos))) { }

void Entity::on_update() {
    for (auto& comp : m_comps) {
        comp->on_update();
    }
}

void Entity::on_draw(DrawSurface& surface) {
    for (auto& comp : m_comps) {
        comp->on_draw(surface);
    }
}

void Entity::destroy() {
    m_destroyed = true;
}
