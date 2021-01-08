#ifndef MATH_H
#define MATH_H

namespace Math {

template<class FloatT>
decltype(auto) lerp(FloatT a, FloatT b, FloatT f) {
    return a + f * (b - a);
}

template<class T>
decltype(auto) abs(T a) {
    return a < 0 ? -a : a;
}

}

using namespace Math;

#endif // MATH_H
