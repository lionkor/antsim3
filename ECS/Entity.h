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

    void on_cleanup(DrawSurface&);

public:
    Entity(const vec<double>& pos = { 0, 0 });

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
    virtual void destroy() final;
    virtual bool marked_destroyed() const final { return m_destroyed; }

    TransformComponent&       transform() { return m_transform; }
    const TransformComponent& transform() const { return m_transform; }
};

#endif // GAMEOBJECT_H
