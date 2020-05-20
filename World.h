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

    static World::Pointer create();

    void     add_object(PhysicalObject* obj);
    RayHit&& try_hit(const vec<double>& pos);
    auto     begin() { return m_objects.begin(); }
    auto     begin() const { return m_objects.begin(); }
    auto     end() { return m_objects.begin(); }
    auto     end() const { return m_objects.begin(); }

    void extend_selection(const PhysicalObject*);
    void reduce_selection(const PhysicalObject*);

    void update(GameWindow&);

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // WORLD_H
