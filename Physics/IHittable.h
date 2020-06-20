#ifndef IHITTABLE_H
#define IHITTABLE_H

#include <cstdint>
#include <functional>

#include "vec.h"

/// \brief The IHittable interface
/// provides support for the inheriting Object to be hit by a Ray
class IHittable
{
public:
    std::size_t layer { 0 };

    IHittable() { }
    virtual ~IHittable() { }

    virtual void on_hit(const vecd&) { }
    virtual bool is_hit(const vecd&) const = 0;

    bool operator<(const IHittable& other) const {
        return layer < other.layer;
    }
};

#endif // IHITTABLE_H
