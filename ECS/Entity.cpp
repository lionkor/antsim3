#include "Entity.h"
#include "Core/World.h"

void Entity::on_cleanup(DrawSurface& surface) {
    // called by the World, at this point m_children is already cleared
    for (auto& comp : m_comps) {
        comp->on_cleanup(surface);
    }
}

WeakPtr<Entity> Entity::add_child(Entity*&& entity) {
    auto weak_ptr = m_world->add_entity(std::move(entity));
    // if the entity already has a parent then something is wack
    auto ptr = weak_ptr.lock();
    ASSERT(!ptr->m_parent);
    ptr->m_parent = this;
    ptr->m_transform.m_relative_to = &m_transform;
    m_children.push_back(ptr.get());
    return weak_ptr;
}

void Entity::remove_child(Entity* entity) {
    auto found = std::find(m_children.begin(), m_children.end(), entity);
    if (found != m_children.end())
        m_children.erase(found);
}

Entity::Entity(const vec<double>& pos)
    // all entitie have a transform component
    : m_transform(add_component(new TransformComponent(pos))) { }


void Entity::on_update() {
    for (auto& child : m_children) {
        report("{}", child->transform());
    }
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
    auto ss = Object::to_stream();
    if (m_parent) {
        ss << "parent=" << *m_parent << ";";
    } else {
        ss << "parent=None;";
    }
    ss << "children_count=" << m_children.size() << ";";
    ss << "components={";
    for (auto& comp : m_comps) {
        ss << *comp;
        if (comp != m_comps.back())
            ss << ",";
    }
    ss << "};";
    return ss;
}
