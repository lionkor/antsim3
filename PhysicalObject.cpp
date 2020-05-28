#include "PhysicalObject.h"
#include "World.h"

PhysicalObject::PhysicalObject(const vec<double> pos, const vec<double> size, World& world)
    : m_pos(pos)
    , m_size(size)
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

void PhysicalObject::set_position(const vec<double>& pos) {
    m_pos         = pos;
    m_has_changed = true;
}

void PhysicalObject::set_size(const vec<double>& size) {
    m_size        = size;
    m_has_changed = true;
}

void PhysicalObject::update() {
}

void PhysicalObject::draw(DrawSurface& surface) {
    // TODO: Abstract this away somehow
    if (!m_has_index) {
        m_index     = surface.draw_new_rectangle(Rectangle(m_pos, m_size));
        m_has_index = true;
    }
    if (m_has_changed) {
        surface.update_rectangle(m_index, Rectangle(m_pos, m_size));
        m_has_changed = false;
    }
}

bool PhysicalObject::is_hit(const vec<double>& pos) const {
    return pos.x >= m_pos.x && pos.x <= m_pos.x + m_size.x && pos.y >= m_pos.y && pos.y <= m_pos.y + m_size.y;
}

void PhysicalObject::on_hit(const vec<double>&) {
    report_trace("HIT!");
    m_has_changed = true;
}

std::stringstream PhysicalObject::to_stream() const {
    auto ss = Object::to_stream();
    ss << "pos=" << m_pos << ";"
       << "size=" << m_size << ";"
       << "has_index=" << m_has_index << ";"
       << "index=" << m_index << ";"
       << "changed=" << m_has_changed << ";"
       << "world=" << m_world << ";";
    return ss;
}
