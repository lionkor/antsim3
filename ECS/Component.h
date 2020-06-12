#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Object.h"
#include "Physics/vec.h"
#include "Rendering/Draw.h"
#include "Rendering/SimpleDrawable.h"

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
protected:
    vec<double> m_position;
    double      m_rotation;

public:
    TransformComponent(const vec<double>& pos = { 0.0, 0.0 }, double rot = 0)
        : m_position(pos)
        , m_rotation(rot) {
    }

    TransformComponent(const TransformComponent&) = default;
    TransformComponent(TransformComponent&&)      = default;
    TransformComponent& operator=(const TransformComponent&) = default;
    TransformComponent& operator=(TransformComponent&&) = default;

    const vec<double>& position() const { return m_position; }
    const double&      rotation() const { return m_rotation; }

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
    SpriteComponent(const vec<double>& relative_position, const vec<double>& sprite_size);

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
