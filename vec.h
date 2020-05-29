#ifndef VEC_H
#define VEC_H

#include "DebugTools.h"
#include "Object.h"
#include <type_traits>
#include <tuple>
#include <ostream>

/// Minimal 2D-vector struct.
template<typename T>
struct vec : public CopyableObject {
    OBJECT(vec)

    T x;
    T y;

    template<typename Vector2DT>
    vec(const Vector2DT& v)
        : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) { }

    vec(T _x, T _y)
        : x(static_cast<T>(_x)), y(static_cast<T>(_y)) { }

    explicit vec(T val)
        : x(static_cast<T>(val)), y(static_cast<T>(val)) { }

    inline std::tuple<T, T> get() const { return std::tie(x, y); }

    template<class IndexT>
    inline T& operator[](IndexT index) { return index == static_cast<IndexT>(0) ? x : y; }

    template<class IndexT>
    inline T operator[](IndexT index) const { return index == static_cast<IndexT>(0) ? x : y; }

    inline bool operator==(const vec& v) const { return std::tie(x, y) == std::tie(v.x, v.y); }
    inline bool operator!=(const vec& v) const { return !(*this == v); }

    inline bool operator<(const vec& v) const { return std::tie(x, y) < std::tie(v.x, v.y); }
    inline bool operator>(const vec& v) const { return std::tie(x, y) > std::tie(v.x, v.y); }
    inline bool operator<=(const vec& v) const { return std::tie(x, y) <= std::tie(v.x, v.y); }
    inline bool operator>=(const vec& v) const { return std::tie(x, y) >= std::tie(v.x, v.y); }

    inline vec<T> operator+(const vec& v) const { return vec<T>(x + v.x, y + v.y); }
    inline vec<T> operator-(const vec& v) const { return vec<T>(x - v.x, y - v.y); }
    template<typename AnyNumberT>
    inline vec<T> operator*(const AnyNumberT& i) const { return vec<T>(x * i, y * i); }
    template<typename AnyNumberT>
    inline vec<T> operator/(const AnyNumberT& i) const { return vec<T>(x / i, y / i); }

    inline vec<T> operator-() const { return vec<T>(-x, -y); }

    inline vec<T>& operator+=(const vec& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    inline vec<T>& operator-=(const vec& v) {
        return *this += -v;
    }
    template<typename AnyNumberT>
    inline vec<T>& operator*=(const AnyNumberT& i) {
        x *= i;
        y *= i;
        return *this;
    }
    template<typename AnyNumberT>
    inline vec<T>& operator/=(const AnyNumberT& i) {
        return *this *= static_cast<AnyNumberT>(1) / i;
    }

    // Object interface
public:
    virtual std::stringstream to_stream() const override {
        auto ss = CopyableObject::to_stream();
        ss << "x=" << x << ";"
           << "y=" << y << ";";
        return ss;
    }
    virtual bool operator==(const Object&) const override { return false; }
    virtual bool operator!=(const Object&) const override { return true; }
};

#endif // VEC_H
