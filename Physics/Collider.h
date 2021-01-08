#ifndef COLLIDER_H
#define COLLIDER_H

#include "Utils/DebugTools.h"
#include "Core/Object.h"
#include "Utils/Result.h"
#include "vec.h"

class Collider
    : public Object
{
    OBJNAME(Collider)

public:
    enum class Shape
    {
        Rectangle,
        Circle,
    };

private:
    std::vector<vecd> m_points { vecd { 0.0 }, vecd { 1.0 }, vecd { 2.0 } }; // collider vertices in order
    double m_radius { 20 };
    Shape m_shape;

public:
    Collider(const Collider::Shape& shape = Shape::Rectangle);

    //    OkResult set_points(const std::vector<vecd>& points);
    //    OkResult set_radius(double radius);

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // COLLIDER_H
