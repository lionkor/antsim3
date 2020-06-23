// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef PHYSICAL_OBJNAME_H
#define PHYSICAL_OBJNAME_H

#include "Core/Object.h"
#include "IHittable.h"
#include "Rendering/Draw.h"
#include "vec.h"

class World;

/// Interface inherited from by all objects in the World.
/// PhysicalObjects are hittable.
class PhysicalObject
    : public Object,
      public IHittable
{
    OBJNAME(PhysicalObject)

private:
    bool m_is_destroyed { false };

protected:
    vecd  m_pos;
    vecd  m_size;
    std::size_t  m_index;
    bool         m_has_index { false };
    mutable bool m_has_changed { true };
    World&       m_world;
    
    /// An update is 1 tick in the engine.
    virtual void update();
    /// Draws this object on the given surface. Usually called by World.
    virtual void draw(DrawSurface& surface);

public:
    /// Construct a new object at that position in the world.
    PhysicalObject(const vecd pos, const vecd size, World& world);
    
    virtual ~PhysicalObject() {
    }

    /// Marks this object as dead. The object will stop being displayed or updated
    /// and will eventually be cleaned up by the World.
    virtual void destroy() final;
    /// Whether the object is destroyed.
    virtual bool destroyed() const final;
    /// Calls update if the object can currently be updated.
    virtual void try_update() final;
    /// Calls draw if the object can currently be drawn.
    virtual void try_draw(DrawSurface& surface) final;

    /// Position in the World.
    vecd& position() { return m_pos; }
    /// Physical size of the object.
    vecd& size() { return m_size; }
    /// Position in the World.
    const vecd& position() const { return m_pos; }
    /// Physical size of the object.
    const vecd& size() const { return m_size; }

    /// Sets the position within the world.
    void set_position(const vecd& pos);
    /// Sets the physical size.
    void set_size(const vecd& size);

    // IHittable
public:
    /// Whether a Ray at given position would hit this object. Use `on_hit` to
    /// do stuff when the object is hit, as this object might get hit but is actually
    /// obscured or otherwise can't actually be hit at this time.
    virtual bool is_hit(const vecd& pos) const override;
    /// Callback called whenever the object is actually hit.
    virtual void on_hit(const vecd&) override;

    // Object interface
    virtual std::stringstream to_stream() const override;
};

#endif // PHYSICAL_OBJNAME_H

