// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>
#include <set>
#include <optional>
#include <chrono>

#include "Utils/DebugTools.h"
#include "Utils/Managed.h"
#include "Core/Object.h"
#include "Physics/vec.h"
#include "Physics/IHittable.h"
#include "Core/GameWindow.h"

#include <deque>

class RayHit;
class Entity;

/// The world manages all physical objects.
class World
    : public Object
{
    OBJNAME(World)

    friend class Application;

private:
    // FIXME: This should really be a map, but maybe one that isn't slow?
    std::vector<SharedPtr<Entity>> m_entities;
    std::vector<SharedPtr<Entity>> m_entities_to_add;
    std::size_t                    m_update_interval_ms;

    class Application& m_application;

    sf::Clock m_update_timer;

    void cleanup_destroyed(DrawSurface&);
    void add_new_entities();

public:
    World(Application& app);
    virtual ~World() { }

    /// Takes ownership of the passed (new-allocated) pointer `obj`.
    WeakPtr<Entity> add_entity(const vecd& pos = {});
    WeakPtr<Entity> add_entity(const Entity& entity);
    /// Generates a Ray at the position, returns the RayHit describing what was hit.
    RayHit try_hit(const vecd& pos);

    void set_update_interval(std::size_t ms) {
        m_update_interval_ms = ms;
    }

    Application&       application() { return m_application; }
    const Application& application() const { return m_application; }

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
