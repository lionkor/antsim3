﻿// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef GAMEOBJNAME_H
#define GAMEOBJNAME_H

#include <vector>
#include <concepts>
#ifdef CPP_20
#include <utility>
#endif // CPP_20

#include "Core/Object.h"
#include "Core/World.h"
#include "Component.h"
#include "Utils/Managed.h"
#include "Rendering/Draw.h"

/// Monolithic class describing any Entity. Uses composition.
class Entity final
    : public Object
{
    OBJNAME(Entity)

    friend class World;

private:
    class World&                            m_world;
    std::vector<std::shared_ptr<Component>> m_comps;
    TransformComponent&                     m_transform;
    bool                                    m_destroyed { false };

    // FIXME: This could/should be WeakPtr
    std::vector<Entity*> m_children;
    Entity*              m_parent { nullptr };

    void on_cleanup(DrawSurface&);

public:
    Entity(World& world, const vecd& pos = { 0, 0 });

    /// Adds the Entity as a child, sets its parent pointer,
    /// registers it with the World (no World::add_entity needed).
    template<typename... Args>
    WeakPtr<Entity> add_child(Args&&... args) {
        auto weak_ptr = m_world.add_entity(std::forward<Args>(args)...);
        // if the entity already has a parent then something is wack
        auto ptr = weak_ptr.lock();
        ASSERT(!ptr->m_parent);
        ptr->m_parent                       = this;
        ptr->m_transform.m_parent_transform = &m_transform;
        m_children.push_back(ptr.get());
        return weak_ptr;
    }
    void remove_child(Entity* entity);

    template<class DerivedComponentT>
#ifdef CPP_20
    requires std::derived_from<DerivedComponentT, Component>
#endif // CPP_20
        bool has_component() const {
        const std::string name = DerivedComponentT::class_name_static();
        auto              iter = std::find_if(m_comps.begin(), m_comps.end(), [&](const auto& comp) {
            return comp->class_name() == name;
        });
        return iter != m_comps.end();
    }

    template<class DerivedComponentT>
#ifdef CPP_20
    requires std::derived_from<DerivedComponentT, Component> 
#endif // CPP_20
    DerivedComponentT* fetch_component() {
        const std::string name = DerivedComponentT::class_name_static();
        auto              iter = std::find_if(m_comps.begin(), m_comps.end(), [&](const auto& comp) {
            return comp->class_name() == name;
        });
        if (iter != m_comps.end()) {
            return reinterpret_cast<DerivedComponentT*>((*iter).get());
        } else {
            return nullptr;
        }
    }

    template<class DerivedComponentT, typename... Args>
#ifdef CPP_20
    requires(std::derived_from<DerivedComponentT, Component> && !std::is_same_v<DerivedComponentT, Component>)
#endif // CPP_20
        DerivedComponentT& add_component(Args&&... args) {
        auto comp = std::make_shared<DerivedComponentT>(*this, std::forward<Args>(args)...);
        if (comp->is_unique() && has_component<DerivedComponentT>()) {
            report_error("Attempted to add Unique component with uuid {} to entity uuid {}, but another component of this type ({}) exists already, this is considered a fatal error", comp->uuid(), this->uuid(), comp->class_name());
            throw std::runtime_error("Attempt to add component that already exists and has the Unique flag");
        }
        m_comps.push_back(std::move(comp));
        auto& ref = *m_comps.back();
        //report("Added component: {}", ref);
        return dynamic_cast<DerivedComponentT&>(ref);
    }

    void         on_update();
    void         on_draw(DrawSurface&);
    void         on_mouse_down(GameWindow&, const HID::MouseAction&);
    void         on_mouse_up(GameWindow&, const HID::MouseAction&);
    void         on_mouse_move(GameWindow&, const HID::MouseAction&);
    void         on_key_down(GameWindow&, const HID::Key&);
    void         on_key_up(GameWindow&, const HID::Key&);
    bool         has_parent() const { return static_cast<bool>(m_parent); }
    virtual void destroy() final;
    virtual bool is_marked_destroyed() const final { return m_destroyed; }

    World& world() {
        return m_world;
    }
    const World& world() const {
        return m_world;
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

#endif // GAMEOBJNAME_H
