// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Object.h"
#include "Physics/vec.h"
#include "Rendering/Draw.h"
#include "Rendering/SimpleDrawable.h"
#include "Core/HID.h"

class Entity;
class ResourceManager;

class Component
    : public Object
{
    OBJNAME(Component)
    friend class Entity;

private:
    Entity&          m_parent;
    ResourceManager& m_resource_manager;

protected:
    /// Called just before the component is destructed, is passed the draw
    /// surface to enable drawing components to unregister with the draw
    /// surface.
    virtual void on_cleanup(DrawSurface&) { }

    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_down { nullptr };
    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_up { nullptr };
    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_move { nullptr };

public:
    Component(Entity&);
    virtual ~Component() noexcept = default;

    virtual bool is_unique() const { return true; }

    virtual void on_update() { }
    virtual void on_draw(DrawSurface&) { }

    Entity&                parent() noexcept { return m_parent; }
    const Entity&          parent() const noexcept { return m_parent; }
    ResourceManager&       resource_manager() noexcept { return m_resource_manager; }
    const ResourceManager& resource_manager() const noexcept { return m_resource_manager; }

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

class TransformComponent
    : public Component
{
    OBJNAME(TransformComponent)

    friend class Entity;

protected:
    vecd m_position;
    double      m_rotation;

    /// Transform of the parent, set by the parent when assigned as a child
    TransformComponent* m_parent_transform { nullptr };

public:
    TransformComponent(Entity& e, const vecd& pos = { 0.0, 0.0 }, double rot = 0)
        : Component(e)
        , m_position(pos)
        , m_rotation(rot) {
    }

    /// Returns {0,0} if no parent
    vecd parent_position() const { return m_parent_transform ? m_parent_transform->position() : vecd(0, 0); }
    /// Returns 0 if no parent
    double parent_rotation() const { return m_parent_transform ? m_parent_transform->rotation() : 0; }

    /// Absolute position
    vecd position() const { return m_position + parent_position(); }
    /// Absolute rotation
    double rotation() const { return m_rotation + parent_rotation(); }

    /// Position relative to parent
    const vecd& relative_position() const { return m_position; }
    /// Rotation relative to parent
    double relative_rotation() const { return m_rotation; }

    // FIXME: These setters are not safe since we work with relative positons and rotations
    void set_position(const vecd& pos) { m_position = pos; }
    void set_position(vecd&& pos) { m_position = std::move(pos); }
    void set_rotation(double rot) { m_rotation = rot; }
    void set_rotation(double&& rot) { m_rotation = std::move(rot); }

    void move_by(const vecd& delta);

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

class SpriteComponent
    : public Component
{
    OBJNAME(SpriteComponent)
private:
    vecd    m_sprite_pos;
    vecd    m_sprite_size;
    Color          m_sprite_background_color;
    std::string    m_texture_name;
    sf::Texture    m_texture;
    bool           m_texture_loaded { false };
    bool           m_initialized { false };
    vecd    m_cached_pos;
    SimpleDrawable m_drawable;
    // TODO sprite / texture

public:
    SpriteComponent(Entity& e, const vecd& parent_position, const vecd& sprite_size, const Color& color = Color::Green, const std::string& texture_name = "");

    std::string texture_name() const { return m_texture_name; }

    // Component interface
public:
    virtual void on_update() override;
    virtual void on_draw(DrawSurface&) override;

    virtual bool is_unique() const override { return false; }

protected:
    virtual void on_cleanup(DrawSurface&) override;

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // COMPONENT_H
