#ifndef GAMEOBJNAME_H
#define GAMEOBJNAME_H

#include <vector>
#include <utility>

#include "Core/Object.h"
#include "Core/World.h"
#include "Component.h"
#include "Utils/Managed.h"
#include "Rendering/Draw.h"
#include "Utils/Constants.h"

/// Monolithic class describing any Entity. Uses composition.
class Entity final
    : public Object
{
    OBJNAME(Entity)

    friend class World;

private:
    class World& m_world;
    std::vector<std::shared_ptr<Component>> m_comps;
    TransformComponent& m_transform;
    bool m_destroyed { false };

    // FIXME: This could/should be WeakPtr
    std::vector<Entity*> m_children;
    Entity* m_parent { nullptr };

    void on_cleanup(DrawSurface&);

public:
    Entity(World& world, const vecd& pos = { 0, 0 });

    void remove_child(Entity* entity);

    /// Adds the Entity as a child, sets its parent pointer,
    /// registers it with the World (no World::add_entity needed).
    template<typename... Args>
    WeakPtr<Entity> add_child(Args&&... args);

    template<class DerivedComponentT>
    bool has_component() const;

    template<class DerivedComponentT>
    DerivedComponentT* fetch_component();

    template<class DerivedComponentT, typename... Args>
    DerivedComponentT& add_component(Args&&...);

    void on_update(float dt);
    void on_draw(DrawSurface&);
    void on_mouse_down(GameWindow&, const HID::MouseAction&);
    void on_mouse_up(GameWindow&, const HID::MouseAction&);
    void on_mouse_move(GameWindow&, const HID::MouseAction&);
    void on_key_down(GameWindow&, const HID::Key&);
    void on_key_up(GameWindow&, const HID::Key&);
    bool has_parent() const { return static_cast<bool>(m_parent); }
    virtual void destroy() final;
    virtual bool is_marked_destroyed() const final { return m_destroyed; }

    World& world() {
        return m_world;
    }
    const World& world() const {
        return m_world;
    }

    std::vector<Entity*>& children() { return m_children; }
    const std::vector<Entity*>& children() const { return m_children; }
    Entity* parent() { return m_parent; }
    const Entity* parent() const { return m_parent; }
    TransformComponent& transform() { return m_transform; }
    const TransformComponent& transform() const { return m_transform; }

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

// templates below, ugly ahead!

template<typename... Args>
WeakPtr<Entity> Entity::add_child(Args&&... args) {
    auto weak_ptr = m_world.add_entity(std::forward<Args>(args)...);
    // if the entity already has a parent then something is wack
    auto ptr = weak_ptr.lock();
    ASSERT(!ptr->m_parent);
    ptr->m_parent = this;
    ptr->m_transform.m_parent_transform = &m_transform;
    m_children.push_back(ptr.get());
    report("added child {}", *ptr);
    return weak_ptr;
}
void remove_child(Entity* entity);

template<class DerivedComponentT>
bool Entity::has_component() const {
    const std::string name = DerivedComponentT::class_name_static();
    const auto has_this_name = [&](const auto& obj_ptr) { return obj_ptr->class_name() == name; };
    return std::find_if(m_comps.begin(), m_comps.end(), has_this_name) != m_comps.end();
}

template<class DerivedComponentT>
DerivedComponentT* Entity::fetch_component() {
    const std::string name = DerivedComponentT::class_name_static();
    const auto has_this_name = [&](const auto& obj_ptr) { return obj_ptr->class_name() == name; };
    decltype(m_comps)::const_iterator iter = std::find_if(m_comps.begin(), m_comps.end(),
        [&](const auto& comp) {
            return comp->class_name() == name;
        });
    if (iter != m_comps.end()) {
        return reinterpret_cast<DerivedComponentT*>((*iter).get());
    } else {
        return nullptr;
    }
}

template<class DerivedComponentT, typename... Args>
[[nodiscard]] DerivedComponentT& Entity::add_component(Args&&... args) {
    auto raw_ptr = new DerivedComponentT(*this, std::forward<Args>(args)...);
    auto comp = SharedPtr<DerivedComponentT>(std::move(raw_ptr));
    if (comp->is_unique() && has_component<DerivedComponentT>()) {
        report_error(cst_error_multiple_unique_components_fmt, comp->uuid(), this->uuid(), comp->class_name());
        throw std::runtime_error(fmt::format(cst_error_multiple_unique_components));
    }
    m_comps.emplace_back(std::move(comp));
    Component& ref = *m_comps.back();
    report("Added component: {}", ref);
    return dynamic_cast<DerivedComponentT&>(ref);
}

#endif // GAMEOBJNAME_H
