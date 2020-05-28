#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "DebugTools.h"
#include "PhysicalObject.h"

/// PhysicsObject is a Physical Object that is affected by physics (rigid body)
class PhysicsObject
    : public PhysicalObject
{
    OBJECT(PhysicsObject)

protected:
    vec<double> m_vel { 0.0, 0.0 };

public:
    
    // Object interface
public:
    virtual std::stringstream to_stream() const override;

    // PhysicalObject interface
protected:
    virtual void update() override;
};

#endif // PHYSICSOBJECT_H
