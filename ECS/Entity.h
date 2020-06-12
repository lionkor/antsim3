#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

#include "Core/Object.h"
#include "Component.h"
#include "Utils/Managed.h"
#include "Rendering/Draw.h"

/// Monolithic class describing any Entity. Uses composition.
class Entity final
    : public Object
{
    OBJECT(Entity)

    friend class World;

private:
    std::vector<std::shared_ptr<Component>> m_comps;
    TransformComponent&                     m_transform;
    bool                                    m_destroyed { false };

    // FIXME: This could/should be WeakPtr
    std::vector<Entity*> m_children;
    Entity*              m_parent { nullptr };
    class World*         m_world { nullptr };

    void on_cleanup(DrawSurface&);

public:
    Entity(const vec<double>& pos = { 0, 0 });

    /// Adds the Entity as a child, sets its parent pointer,
    /// registers it with the World (no World::add_entity needed).
    WeakPtr<Entity> add_child(Entity*&& entity);
    void            remove_child(Entity* entity);

    template<class DerivedComponentT>
    requires std::derived_from<DerivedComponentT, Component> bool has_component() const {
        const std::string name = DerivedComponentT().class_name();
        auto              iter = std::find_if(m_comps.begin(), m_comps.end(), [&](const auto& comp) {
            return comp.class_name() == name;
        });
        return iter != m_comps.end();
    }

    template<class DerivedComponentT>
    requires(std::derived_from<DerivedComponentT, Component> && !std::is_same_v<DerivedComponentT, Component>) DerivedComponentT& add_component(DerivedComponentT*&& comp) {
        m_comps.push_back(std::shared_ptr<DerivedComponentT>(std::move(comp)));
        auto& ref = *m_comps.back();
        // set parent
        ref.m_parent = this;
        report("Added component: {}", ref);
        return dynamic_cast<DerivedComponentT&>(ref);
    }

    void         on_update();
    void         on_draw(DrawSurface&);
    void         on_mouse_down(GameWindow&, const HID::MouseAction&);
    void         on_mouse_up(GameWindow&, const HID::MouseAction&);
    void         on_mouse_move(GameWindow&, const HID::MouseAction&);
    bool         has_world() const { return static_cast<bool>(m_world); }
    bool         has_parent() const { return static_cast<bool>(m_parent); }
    virtual void destroy() final;
    virtual bool is_marked_destroyed() const final { return m_destroyed; }

    World& world() {
        ASSERT(m_world);
        return *m_world;
    }
    const World& world() const {
        ASSERT(m_world);
        return *m_world;
    }

    std::vector<Entity*>&       children() { return m_children; }
    const std::vector<Entity*>& children() const { return m_children; }
    Entity*                     parent() { return m_parent; }
    const Entity*               parent() const { return m_parent; }
    TransformComponent&         transform() { return m_transform; }
    const TransformComponent&   transform() const { return m_transform; }

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // GAMEOBJECT_H
