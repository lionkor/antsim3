#include "Entity.h"
#include "Core/World.h"

void Entity::on_cleanup(DrawSurface& surface) {
    // called by the World, at this point m_children is already cleared
    for (auto& comp : m_comps) {
        comp->on_cleanup(surface);
    }
}

void Entity::remove_child(Entity* entity) {
    auto found = std::find(m_children.begin(), m_children.end(), entity);
    if (found != m_children.end())
        m_children.erase(found);
}

Entity::Entity(World& world, const vecd& pos)
    // all entitie have a transform component
    : m_world(world)
    , m_transform(add_component<TransformComponent>(pos)) { }


void Entity::on_update(float dt) {
    for (auto& comp : m_comps) {
        comp->on_update(dt);
    }
}

void Entity::on_draw(DrawSurface& surface) {
    for (auto& comp : m_comps) {
        comp->on_draw(surface);
    }
}

void Entity::on_mouse_down(GameWindow& window, const HID::MouseAction& action) {
    for (auto& comp : m_comps) {
        if (comp->on_mouse_down)
            comp->on_mouse_down(window, action);
    }
}

void Entity::on_mouse_up(GameWindow& window, const HID::MouseAction& action) {
    for (auto& comp : m_comps) {
        if (comp->on_mouse_up)
            comp->on_mouse_up(window, action);
    }
}

void Entity::on_mouse_move(GameWindow& window, const HID::MouseAction& action) {
    for (auto& comp : m_comps) {
        if (comp->on_mouse_move)
            comp->on_mouse_move(window, action);
    }
}

void Entity::destroy() {
    /*
     * When an Entity is destroyed, it is marked for deletion.
     * The World that the Entity is part of will then 
     */
    report("destroy called on {}", uuid());
    if (m_parent && !m_parent->is_marked_destroyed()) {
        m_parent->remove_child(this);
    }
    m_destroyed = true;
    // recursively call destroy() on children
    for (auto& child : m_children) {
        child->m_parent = nullptr;
        child->destroy();
    }
    m_children.clear();
}

std::stringstream Entity::to_stream() const {
    TS_BEGIN(Object);
    TS_PROP_M(m_parent);
    TS_PROP_M(m_children.size());
    ss << "components={";
    for (auto& comp : m_comps) {
        ss << *comp;
        if (comp != m_comps.back())
            ss << ",";
    }
    ss << "};";
    TS_END();
}

void Entity::on_key_down(GameWindow& window, const HID::Key& key) {
    for (auto& comp : m_comps) {
        if (comp->on_key_down)
            comp->on_key_down(window, key);
    }
}

void Entity::on_key_up(GameWindow& window, const HID::Key& key) {
    for (auto& comp : m_comps) {
        if (comp->on_key_up)
            comp->on_key_up(window, key);
    }
}
