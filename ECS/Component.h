#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Object.h"

class Entity;

class Component
    : public Object
{
    OBJECT(Component)
    friend class Entity;

    Entity* m_parent;

public:
    Component()                   = default;
    virtual ~Component() noexcept = default;

    Component(const Component& other);
    Component(Component&& other) noexcept = default;

    bool has_parent() const noexcept { return static_cast<bool>(m_parent); }
};

#endif // COMPONENT_H
