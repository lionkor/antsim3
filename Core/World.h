#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>
#include <set>
#include <boost/ref.hpp>
#include <boost/optional.hpp>

#include "Utils/DebugTools.h"
#include "Utils/Managed.h"
#include "Core/Object.h"
#include "Physics/vec.h"
#include "Physics/IHittable.h"
#include "Physics/PhysicalObject.h"
#include "Core/GameWindow.h"

class RayHit;


/// The world manages all physical objects.
class World
    : public Object
{
    OBJECT(World)

private:
    // FIXME: This should really be a map, but maybe one that isn't slow?
    std::vector<Managed<PhysicalObject>> m_objects;
    std::set<const PhysicalObject*>              m_selected_objects;

public:
    World();
    virtual ~World() { }

    /// Takes ownership of the passed (new-allocated) pointer `obj`.
    void add_object(PhysicalObject*&& obj);
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
