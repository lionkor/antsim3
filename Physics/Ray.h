// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef RAY_H
#define RAY_H

#include <vector>

#include "Core/Object.h"
#include "vec.h"
#include "IHittable.h"

class PhysicalObject;


/// Describes what has been hit, in what order.
class RayHit : public Object
{
    OBJNAME(RayHit);

    std::vector<IHittable*> m_hits_in_order;

public:
    RayHit() { }

    /// Called by Ray when an object has been hit. Inserts the object into the hit-list
    /// according to their "depth" or "layer" in the scene.
    void add_hit(IHittable* hittable) {
        m_hits_in_order.push_back(hittable);
        std::sort(m_hits_in_order.begin(), m_hits_in_order.end(), [&](auto& h1, auto& h2) { return *h1 < *h2; });
    }

    /// Begin of the list of hits.
    auto begin() { return m_hits_in_order.begin(); }
    /// End of the list of hits.
    auto end() { return m_hits_in_order.end(); }
};

/// Helper class for calculating raycasts used for hit detection.
class Ray : public Object
{
    OBJNAME(Ray)
    vecd m_pos;
    RayHit m_hit;

public:
    /// Sets up a new raycast at the position.
    Ray(const vecd& pos)
        : m_pos(pos) { }

    /// Fires the Ray at the object. On hit, the hit object will be added
    /// to the internal RayHit instance.
    void try_intersect(PhysicalObject& obj);
    /// Get the results of the raycast operation. Invalidates this Ray.
    RayHit&& result();

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // RAY_H
