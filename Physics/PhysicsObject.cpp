#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const vec<double>& pos, const vec<double>& size, World& world)
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
