#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Object.h"
#include "Physics/vec.h"
#include "Rendering/Draw.h"
#include "Rendering/SimpleDrawable.h"
#include "Core/HID.h"

class Entity;

class Component
    : public Object
{
    OBJECT(Component)
    friend class Entity;

private:
    Entity* m_parent;

protected:
    /// Called just before the component is destructed, is passed the draw
    /// surface to enable drawing components to unregister with the draw
    /// surface.
    virtual void on_cleanup(DrawSurface&) { }

    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_down { nullptr };
    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_up { nullptr };
    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_move { nullptr };
public:

    Component()                   = default;
    virtual ~Component() noexcept = default;

    bool has_parent() const noexcept { return static_cast<bool>(m_parent); }

    virtual void on_update() { }
    virtual void on_draw(DrawSurface&) { }

    Entity*       parent() noexcept { return m_parent; }
    const Entity* parent() const noexcept { return m_parent; }

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

class TransformComponent
    : public Component
{
    OBJECT(TransformComponent)

    friend class Entity;

protected:
    vec<double> m_position;
    double      m_rotation;

    /// Transform of the parent, set by the parent when assigned as a child
    TransformComponent* m_parent_transform { nullptr };

public:
    TransformComponent(const vec<double>& pos = { 0.0, 0.0 }, double rot = 0)
        : m_position(pos)
        , m_rotation(rot) {
    }

    TransformComponent(const TransformComponent&) = default;
    TransformComponent(TransformComponent&&)      = default;
    TransformComponent& operator=(const TransformComponent&) = default;
    TransformComponent& operator=(TransformComponent&&) = default;

    /// Returns {0,0} if no parent
    vec<double> parent_position() const { return m_parent_transform ? m_parent_transform->position() : vec<double>(0, 0); }
    /// Returns 0 if no parent
    double parent_rotation() const { return m_parent_transform ? m_parent_transform->rotation() : 0; }

    /// Absolute position
    vec<double> position() const { return m_position + parent_position(); }
    /// Absolute rotation
    double rotation() const { return m_rotation + parent_rotation(); }

    /// Position relative to parent
    const vec<double>& relative_position() const { return m_position; }
    /// Rotation relative to parent
    double relative_rotation() const { return m_rotation; }

    // FIXME: These setters are not safe since we work with relative positons and rotations
    void set_position(const vec<double>& pos) { m_position = pos; }
    void set_position(vec<double>&& pos) { m_position = std::move(pos); }
    void set_rotation(double rot) { m_rotation = rot; }
    void set_rotation(double&& rot) { m_rotation = std::move(rot); }

    void move_by(const vec<double>& delta);

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

class SpriteComponent
    : public Component
{
    OBJECT(SpriteComponent)
private:
    vec<double> m_sprite_pos;
    vec<double> m_sprite_size;
    bool        m_changed { false };
    bool        m_initialized { false };
    std::size_t m_render_id;
    vec<double> m_cached_pos { 0, 0 };
    // TODO sprite / texture

public:
    SpriteComponent(const vec<double>& parent_position, const vec<double>& sprite_size);

    // Component interface
public:
    virtual void on_update() override;
    virtual void on_draw(DrawSurface&) override;

    // Object interface
public:
    virtual std::stringstream to_stream() const override;

    // Component interface
protected:
    virtual void on_cleanup(DrawSurface&) override;
};

#endif // COMPONENT_H
