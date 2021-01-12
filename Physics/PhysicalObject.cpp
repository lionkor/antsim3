#include "PhysicalObject.h"
#include "Core/World.h"

PhysicalObject::PhysicalObject(const vecd pos, const vecd size, World& world)
    : m_rectangle(pos, size)
    , m_world(world) {
}

void PhysicalObject::destroy() {
    report_function();
    m_is_destroyed = true;
}

bool PhysicalObject::destroyed() const {
    return m_is_destroyed;
}

void PhysicalObject::try_update() {
    if (!m_is_destroyed)
        update();
}

void PhysicalObject::try_draw(DrawSurface& surface) {
    if (!m_is_destroyed)
        draw(surface);
}

void PhysicalObject::set_position(const vecd& pos) {
    m_rectangle.set_position(pos);
}

void PhysicalObject::set_size(const vecd& size) {
    m_rectangle.set_size(size);
}

void PhysicalObject::update() {
}

void PhysicalObject::draw(DrawSurface& surface) {
    // TODO: Abstract this away somehow
    m_rectangle.draw(surface.window());
}

bool PhysicalObject::is_hit(const vecd& pos) const {
    return pos.x >= position().x && pos.x <= position().x + size().x && pos.y >= position().y && pos.y <= position().y + size().y;
}

void PhysicalObject::on_hit(const vecd&) {
    report_trace("HIT!");
}

std::stringstream PhysicalObject::to_stream() const {
    auto ss = Object::to_stream();
    return ss;
}
