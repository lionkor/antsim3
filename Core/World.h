#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>
#include <set>
#include <boost/ref.hpp>
#include <boost/optional.hpp>
#include <chrono>

#include "Utils/DebugTools.h"
#include "Utils/Managed.h"
#include "Core/Object.h"
#include "Physics/vec.h"
#include "Physics/IHittable.h"
#include "ECS/Entity.h"
#include "Core/GameWindow.h"

#include <deque>

class RayHit;


/// The world manages all physical objects.
class World
    : public Object
{
    OBJECT(World)

    friend class Application;

private:
    // FIXME: This should really be a map, but maybe one that isn't slow?
    std::vector<SharedPtr<Entity>> m_entities;
    std::vector<SharedPtr<Entity>> m_entities_to_add;
    std::size_t                    m_update_interval_ms;

    class Application* m_application { nullptr };

    sf::Clock m_update_timer;

    void cleanup_destroyed(DrawSurface&);
    void add_new_entities();

public:
    World();
    virtual ~World() { }

    /// Takes ownership of the passed (new-allocated) pointer `obj`.
    WeakPtr<Entity> add_entity(Entity*&& obj);
    /// Generates a Ray at the position, returns the RayHit describing what was hit.
    RayHit try_hit(const vec<double>& pos);
    /// Begin of the container holding all objects.
    auto begin() { return m_entities.begin(); }
    /// Begin of the container holding all objects.
    auto begin() const { return m_entities.begin(); }
    /// End of the container holding all objects.
    auto end() { return m_entities.end(); }
    /// End of the container holding all objects.
    auto end() const { return m_entities.end(); }
    /// End of the container holding all objects.
    auto rend() { return m_entities.rend(); }
    /// End of the container holding all objects.
    auto rend() const { return m_entities.rend(); }

    void set_update_interval(std::size_t ms) {
        m_update_interval_ms = ms;
    }

    std::vector<SharedPtr<Entity>>&       entities() { return m_entities; }
    const std::vector<SharedPtr<Entity>>& entities() const { return m_entities; }

    /// When fn returns false, the loop breaks.
    void for_each_entity(std::function<bool(SharedPtr<Entity>&)> fn) {
        for (auto& entity_ptr : m_entities) {
            if (!fn(entity_ptr)) {
                break;
            }
        }
    }

    /// Updates the world and calls into the window to update, too.
    void update(GameWindow&);

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // WORLD_H
