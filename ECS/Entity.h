#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

#include "Core/Object.h"
#include "Component.h"

class Entity
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
        return *m_comps.back();
    }
};

#endif // GAMEOBJECT_H
