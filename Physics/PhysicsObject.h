#ifndef PHYSICSOBJNAME_H
#define PHYSICSOBJNAME_H

#include "Utils/DebugTools.h"
#include "PhysicalObject.h"
#include "Collider.h"

/// PhysicsObject is a Physical Object that is affected by physics (rigid body)
class PhysicsObject
    : public PhysicalObject
{
    OBJNAME(PhysicsObject)

protected:
    vecd m_vel { 0.0, 0.0 };
    Collider m_collider;

public:
    PhysicsObject(const vecd& pos, const vecd& size, World& world);

    // Object interface
public:
    virtual std::stringstream to_stream() const override;

    // PhysicalObject interface
protected:
    virtual void update() override;
};

#endif // PHYSICSOBJNAME_H
