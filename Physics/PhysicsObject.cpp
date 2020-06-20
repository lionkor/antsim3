#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const vecd& pos, const vecd& size, World& world)
    : PhysicalObject(pos, size, world) {
}


std::stringstream PhysicsObject::to_stream() const {
    auto ss = PhysicalObject::to_stream();
    ss << "vel=" << m_vel << ";"
       << "collider=" << m_collider << ";";
    return ss;
}

void PhysicsObject::update() {
}
