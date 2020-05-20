#ifndef RAY_H
#define RAY_H

#include "Object.h"
#include "vec.h"
#include <boost/container/vector.hpp>
#include <boost/sort/sort.hpp>
#include "IHittable.h"

class PhysicalObject;

class RayHit : public Object
{
    OBJECT(RayHit);

    boost::container::vector<IHittable*> m_hits_in_order;

public:
    RayHit() { }

    void add_hit(IHittable* hittable) {
        m_hits_in_order.push_back(hittable);
        boost::sort::flat_stable_sort(m_hits_in_order.begin(), m_hits_in_order.end(), [&](auto& h1, auto& h2) { return *h1 < *h2; });
    }

    auto begin() { return m_hits_in_order.begin(); }
    auto end() { return m_hits_in_order.end(); }
};

class Ray : public Object
{
    OBJECT(Ray)
    vec<double> m_pos;
    RayHit      m_hit;

public:
    Ray(const vec<double>& pos)
        : m_pos(pos) { }

    void     try_intersect(PhysicalObject& obj);
    RayHit&& result();

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // RAY_H
