#ifndef WORLD_H
#define WORLD_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/set.hpp>
#include <boost/ref.hpp>
#include <boost/optional.hpp>

#include "DebugTools.h"
#include "Object.h"
#include "vec.h"
#include "IHittable.h"
#include "PhysicalObject.h"
#include "GameWindow.h"

class RayHit;


/// The world manages all physical objects.
class World
    : public Object,
      public boost::enable_shared_from_this<World>
{
    OBJECT(World)

private:
    // FIXME: This should really be a map, but maybe one that isn't slow?
    boost::container::vector<std::unique_ptr<PhysicalObject>> m_objects;
    boost::container::set<const PhysicalObject*>              m_selected_objects;

    World();

public:
    virtual ~World() { }

    typedef boost::shared_ptr<World> Pointer;

    /// Constructs a new world.
    static World::Pointer create();

    /// Takes ownership of the passed (new-allocated) pointer `obj`.
    void add_object(PhysicalObject* obj);
    /// Generates a Ray at the position, returns the RayHit describing what was hit.
    RayHit try_hit(const vec<double>& pos);
    /// Begin of the container holding all objects.
    auto begin() { return m_objects.begin(); }
    /// Begin of the container holding all objects.
    auto begin() const { return m_objects.begin(); }
    /// End of the container holding all objects.
    auto end() { return m_objects.begin(); }
    /// End of the container holding all objects.
    auto end() const { return m_objects.begin(); }

    /// Extends the internal set of selected objects by this object.
    /// Inserting the same object multiple times has no effect.
    void extend_selection(const PhysicalObject*);
    /// Reduces the internal set of selected objects by this object.
    void reduce_selection(const PhysicalObject*);

    /// Updates the world and calls into the window to update, too.
    void update(GameWindow&);

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // WORLD_H
