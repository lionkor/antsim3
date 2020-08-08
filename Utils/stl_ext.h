#ifndef STL_EXT_H
#define STL_EXT_H

#include <algorithm>
#include <execution>

namespace stl_ext {

template<class ContainerT, class ValueT = decltype(ContainerT::value_type)>
static inline bool contains(const ContainerT& container, const ValueT& value) {
    return std::find(container.begin(), container.end(), value) != container.end();
}

template<class ContainerT, class ValueT = decltype(ContainerT::value_type)>
static inline bool contains_par(const ContainerT& container, const ValueT& value) {
    return std::find(std::execution::par_unseq, container.begin(), container.end(), value) != container.end();
}

template<class ContainerT, class ValueT = decltype(ContainerT::value_type)>
static inline bool contains_par_member_find(const ContainerT& container, const ValueT& value) {
    return container.find(std::execution::par_unseq, value) != container.end();
}

}

#endif // STL_EXT_H
