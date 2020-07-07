#ifndef MATH_H
#define MATH_H

template<class FloatT>
decltype(auto) lerp(FloatT a, FloatT b, FloatT f) {
    return a + f * (b - a);
}

#endif // MATH_H
