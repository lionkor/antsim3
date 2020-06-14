#ifndef COMMON_H
#define COMMON_H

#include <cstdint>
#include <functional>

using std::size_t;

template<typename T>
using Ref = std::reference_wrapper<T>;

#define make_ref std::ref

#endif // COMMON_H
