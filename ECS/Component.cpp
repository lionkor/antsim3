#include "Component.h"
#include "Core/World.h"
#include "Core/Application.h"
#include "Entity.h"

std::stringstream Component::to_stream() const {
    TS_BEGIN(Object);
    TS_PROP_M(m_parent->uuid());
    TS_END();
}

void TransformComponent::move_by(const vec<double>& delta) {
    m_position += delta;
}

std::stringstream TransformComponent::to_stream() const {
    TS_BEGIN(Component);
    TS_PROP_M(m_position);
    TS_PROP_M(m_rotation);
    TS_END();
}

SpriteComponent::SpriteComponent(const vec<double>& parent_position, const vec<double>& sprite_size, const Color& color, const std::string& name)
    : m_sprite_pos(parent_position)
    , m_sprite_size(sprite_size)
    , m_sprite_background_color(color)
    , m_texture_name(name)
    , m_drawable(SimpleDrawable::PrimitiveType::Quads, 4) {
}

void SpriteComponent::on_update() {
    if (!has_parent())
        report_error("no parent set for {}", *this);
    if (!m_initialized && !m_texture_name.empty()) {
        report("loading texture {}", m_texture_name);
        auto& resman = parent()->world().application().resource_manager();
        auto  result = resman.get_resource_by_name(m_texture_name);
        if (result.error()) {
            report_error("error getting texture \"{}\": {}", m_texture_name, result.message());
        } else {
            LazyFile& file_ref = result.value().get();
            auto      result   = file_ref.get();
            if (result.error()) {
                report_error("error loading file: {}", result.message());
            } else {
                auto& data = result.value().get();
                bool  rt   = m_texture.loadFromMemory(data.data(), data.size());
                if (!rt) {
                    report_error("texture not loaded properly");
                } else {
                    report("texture loaded!");
                    m_texture_loaded = true;
                }
            }
        }
        m_cached_pos = parent()->transform().position();
    }
    if (m_cached_pos != parent()->transform().position()) {
        m_cached_pos = parent()->transform().position();
        m_drawable.set_changed();
    }
}

void SpriteComponent::on_draw(DrawSurface& surface) {
    if (!m_initialized) {
        const vec<double>& pos = parent()->transform().position();
        Rectangle          rect(pos + m_sprite_pos, m_sprite_size);
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
        const vec<double>& pos = parent()->transform().position();
        Rectangle          rect(pos + m_sprite_pos, m_sprite_size);
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
