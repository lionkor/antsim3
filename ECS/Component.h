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

    bool has_parent() const noexcept { return static_cast<bool>(m_parent); }

    virtual void on_update() { }
    virtual void on_draw() { }
};

#endif // COMPONENT_H
