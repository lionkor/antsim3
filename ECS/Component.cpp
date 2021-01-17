#include "Component.h"
#include "Core/World.h"
#include "Core/Application.h"
#include "Entity.h"

Component::Component(Entity& parent)
    : m_parent(parent)
    , m_resource_manager(m_parent.world().application().resource_manager())
    , m_world(m_parent.world())
    , m_game_window(m_parent.world().application().window())
    , m_application(m_parent.world().application()) {
}

std::stringstream Component::to_stream() const {
    TS_BEGIN(Object);
    TS_PROP_M(m_parent.uuid());
    TS_END();
}

void TransformComponent::move_by(const vecd& delta) {
    m_position += delta;
}

std::stringstream TransformComponent::to_stream() const {
    TS_BEGIN(Component);
    TS_PROP_M(m_position);
    TS_PROP_M(m_rotation);
    TS_END();
}

SpriteComponent::SpriteComponent(Entity& e, const vecd& sprite_offset, const vecd& sprite_size, const Color& color, const std::string& name)
    : Component(e)
    , m_texture_name(name)
    , m_sprite_offset(sprite_offset)
    , m_drawable(sprite_offset, sprite_size, 0) {
    m_drawable.set_color(color);
    if (!name.empty()) {
        m_texture = resource_manager().load_texture(name);
        m_drawable.set_texture(m_texture.get());
    } else {
        m_texture = nullptr;
    }
}

void SpriteComponent::on_update(float) {
    m_drawable.set_position(parent().transform().position() + m_sprite_offset);
}

void SpriteComponent::on_draw(DrawSurface& surface) {
    surface.draw(m_drawable);
}

std::stringstream SpriteComponent::to_stream() const {

    TS_BEGIN(Component);
    TS_PROP_M(m_drawable);
    TS_END();
}

void SpriteComponent::on_cleanup(DrawSurface& surface) {
    // TODO TODO
    (void)surface;
}
