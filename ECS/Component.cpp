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

SpriteComponent::SpriteComponent(Entity& e, const vecd& parent_position, const vecd& sprite_size, const Color& color, const std::string& name)
    : Component(e)
    , m_sprite_pos(parent_position)
    , m_sprite_size(sprite_size)
    , m_sprite_background_color(color)
    , m_texture_name(name)
    , m_drawable(SimpleDrawable::PrimitiveType::Quads, 4) {
}

void SpriteComponent::on_update(float) {
    if (!m_initialized && !m_texture_name.empty()) {
        report("loading texture {}", m_texture_name);
        auto& resman = parent().world().application().resource_manager();
        auto result = resman.get_resource_by_name(m_texture_name);
        if (result.error()) {
            report_error("error getting texture \"{}\": {}", m_texture_name, result.message());
        } else {
            LazyFile& file_ref = result.value().get();
            auto result = file_ref.load();
            if (!result) {
                report_error("error loading file.");
            } else {
                auto& data = *result;
                bool rt = m_texture.loadFromMemory(data.data(), data.size());
                if (!rt) {
                    report_error("texture not loaded properly");
                } else {
                    report("texture loaded!");
                    m_texture_loaded = true;
                }
            }
        }
        m_cached_pos = parent().transform().position();
    }
    if (m_cached_pos != parent().transform().position()) {
        m_cached_pos = parent().transform().position();
        m_drawable.set_changed();
    }
}

void SpriteComponent::on_draw(DrawSurface& surface) {
    if (!m_initialized) {
        const vecd& pos = parent().transform().position();
        Rectangle rect(pos + m_sprite_pos, m_sprite_size);
        if (!m_texture_name.empty()) {
            SimpleDrawable::Vector2f tex_size = SimpleDrawable::Vector2f(m_texture.getSize().x, m_texture.getSize().y);

            m_drawable[0] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x, rect.pos.y), SimpleDrawable::Vector2f(0, 0));
            m_drawable[1] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), SimpleDrawable::Vector2f(tex_size.x, 0));
            m_drawable[2] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), tex_size);
            m_drawable[3] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), SimpleDrawable::Vector2f(0, tex_size.y));
        } else {
            m_drawable[0] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x, rect.pos.y), m_sprite_background_color);
            m_drawable[1] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), m_sprite_background_color);
            m_drawable[2] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), m_sprite_background_color);
            m_drawable[3] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), m_sprite_background_color);
        }
        m_initialized = true;
        if (m_texture_loaded)
            m_drawable.set_texture(&m_texture);
        m_drawable.set_changed();
    }
    if (m_drawable.has_changed()) {
        const vecd& pos = parent().transform().position();
        Rectangle rect(pos + m_sprite_pos, m_sprite_size);
        if (!m_texture_name.empty()) {
            SimpleDrawable::Vector2f tex_size = SimpleDrawable::Vector2f(m_texture.getSize().x, m_texture.getSize().y);

            m_drawable[0] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x, rect.pos.y), SimpleDrawable::Vector2f(0, 0));
            m_drawable[1] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), SimpleDrawable::Vector2f(tex_size.x, 0));
            m_drawable[2] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), tex_size);
            m_drawable[3] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), SimpleDrawable::Vector2f(0, tex_size.y));
        } else {
            m_drawable[0] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x, rect.pos.y), m_sprite_background_color);
            m_drawable[1] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), m_sprite_background_color);
            m_drawable[2] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), m_sprite_background_color);
            m_drawable[3] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), m_sprite_background_color);
        }
    }
    m_drawable.draw(surface);
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
    TS_BEGIN(Component);
    TS_PROP_M(m_sprite_pos);
    TS_PROP_M(m_sprite_size);
    TS_PROP_M(m_initialized);
    TS_PROP_M(m_cached_pos);
    TS_PROP_M(m_drawable);
    TS_END();
}

void SpriteComponent::on_cleanup(DrawSurface& surface) {
    // TODO TODO
}
