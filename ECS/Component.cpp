#include "Component.h"
#include "Entity.h"

std::stringstream Component::to_stream() const {
    auto ss = Object::to_stream();
    ss << "parent=0x" << std::hex << m_parent << ";";
    return ss;
}


void TransformComponent::move_by(const vec<double>& delta) {
    m_position += delta;
}

std::stringstream TransformComponent::to_stream() const {
    auto ss = Component::to_stream();
    ss << "position=" << m_position << ";"
       << "rotation=" << m_rotation << ";";
    return ss;
}


SpriteComponent::SpriteComponent(const vec<double>& relative_position, const vec<double>& sprite_size)
    : m_sprite_pos(relative_position)
    , m_sprite_size(sprite_size) {
}

void SpriteComponent::on_update() {
    if (!has_parent())
        report_error("no parent set for {}", *this);
    if (!m_initialized) {
        m_cached_pos = parent()->transform().position();
        // m_initialized is set to true in on_draw
    }
    if (m_cached_pos != parent()->transform().position()) {
        m_cached_pos = parent()->transform().position();
        m_changed    = true;
    }
}

void SpriteComponent::on_draw(DrawSurface& surface) {
    if (!m_initialized) {
        const vec<double>& pos = parent()->transform().position();
        m_render_id            = surface.draw_new_rectangle(Rectangle(pos + m_sprite_pos, m_sprite_size));
        m_initialized = true;
        //m_changed     = true;
    }
    if (m_changed) {
        const vec<double>& pos = parent()->transform().position();
        surface.update_rectangle(m_render_id, Rectangle(pos + m_sprite_pos, m_sprite_size));
        m_changed = false;
    }
}

std::stringstream SpriteComponent::to_stream() const {
    /*
    m_sprite_pos;
    m_sprite_size;
    m_changed { true };
    m_initialized { false };
    m_render_id;
    m_cached_pos { 0, 0 };
    */
    auto ss = Component::to_stream();
    ss << "sprite_pos=" << m_sprite_pos << ";"
       << "sprite_size=" << m_sprite_size << ";"
       << "changed=" << m_changed << ";"
       << "initialized=" << m_initialized << ";"
       << "render_id=" << m_render_id << ";"
       << "cached_pos=" << m_cached_pos << ";";
    return ss;
}
