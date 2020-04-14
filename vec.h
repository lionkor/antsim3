#ifndef VEC_H
#define VEC_H

/*
 * really simple vector struct for 2d vectors
 */

#include "DebugTools.h"
#include <type_traits>
#include <tuple>

template<typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
struct vec {
    T x;
    T y;

    vec(T _x, T _y)
        : x(_x), y(_y) {}

    vec(T val)
        : x(val), y(val) {}

    inline std::tuple<T, T> get() const { return std::tie(x, y); }

    template<class IndexT, std::enable_if_t<std::is_integral<T>::value, int> = 0>
    inline T& operator[](IndexT index) { return index == static_cast<IndexT>(0) ? x : y; }

    template<class IndexT, std::enable_if_t<std::is_integral<T>::value, int> = 0>
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
};

#endif // VEC_H
