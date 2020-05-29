#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

#include "Core/Object.h"
#include "Component.h"
#include "Utils/Managed.h"

/// Monolithic class describing any Entity. Uses composition.
class Entity final
    : public Object
{
    OBJECT(Entity)
private:
    std::vector<Managed<Component>> m_comps;

public:
    template<class DerivedComponentT>
    requires std::derived_from<DerivedComponentT, Component> bool has_component() const {
        const std::string name = DerivedComponentT().class_name();
        auto              iter = std::find_if(m_comps.begin(), m_comps.end(), [&](const auto& comp) {
            return comp->class_name() == name;
        });
        return iter != m_comps.end();
    }

    template<class DerivedComponentT, typename... Args>
    requires std::derived_from<DerivedComponentT, Component> Component& add_component(Args&&... args) {
        m_comps.push_back(new DerivedComponentT(std::forward<Args>(args)...));
        auto& ref = *m_comps.back();
        // set parent
        ref.m_parent = this;
        return ref;
    }
};

#endif // GAMEOBJECT_H
