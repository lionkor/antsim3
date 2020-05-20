#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include "Object.h"
#include "IHittable.h"
#include "Draw.h"
#include "vec.h"

class World;

/// Interface inherited from by all objects in the World.
/// PhysicalObjects are hittable.
class PhysicalObject
    : public Object,
      public IHittable
{
    OBJECT(PhysicalObject)

protected:
    vec<double>  m_pos;
    vec<double>  m_size;
    std::size_t  m_index;
    bool         m_has_index { false };
    mutable bool m_changed { true };
    World&       m_world;

public:
    /// Construct a new object at that position in the world.
    PhysicalObject(const vec<double> pos, const vec<double> size, World& world);

    /// Position in the World.
    vec<double>&       position() { return m_pos; }
    /// Physical size of the object.
    vec<double>&       size() { return m_size; }
    /// Position in the World.
    const vec<double>& position() const { return m_pos; }
    /// Physical size of the object.
    const vec<double>& size() const { return m_size; }

    /// Sets the position within the world.
    void set_position(const vec<double>& pos);
    /// Sets the physical size.
    void set_size(const vec<double>& size);

    /// Draws this object on the given surface. Usually called by World.
    virtual void draw(DrawSurface& surface);

    // IHittable
public:
    /// Whether a Ray at given position would hit this object. Use `on_hit` to
    /// do stuff when the object is hit, as this object might get hit but is actually
    /// obscured or otherwise can't be hit at this time.
    virtual bool is_hit(const vec<double>& pos) const override;
    /// Callback called whenever the object is actually hit. 
    virtual void on_hit(const vec<double>&) override;

    // Object interface
    virtual std::stringstream to_stream() const override;
};

#endif // PHYSICAL_OBJECT_H
