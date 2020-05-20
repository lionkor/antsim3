#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include "Object.h"
#include "IHittable.h"
#include "Draw.h"
#include "vec.h"

class World;

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
    PhysicalObject(const vec<double> pos, const vec<double> size, World& world);

    vec<double>&       position() { return m_pos; }
    vec<double>&       size() { return m_size; }
    const vec<double>& position() const { return m_pos; }
    const vec<double>& size() const { return m_size; }

    void set_position(const vec<double>& pos);
    void set_size(const vec<double>& size);

    virtual void draw(DrawSurface& surface);

    // IHittable
public:
    virtual bool is_hit(const vec<double>& pos) const override;
    virtual void on_hit(const vec<double>&) override;

    // Object interface
    virtual std::stringstream to_stream() const override;
};

#endif // PHYSICAL_OBJECT_H
