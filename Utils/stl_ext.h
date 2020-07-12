#ifndef STL_EXT_H
#define STL_EXT_H

#include <algorithm>

namespace stl_ext {

template<class ContainerT>
static inline bool contains(const ContainerT& container, typename ContainerT::value_type value) {
    return std::find(container.begin(), container.end(), value) != container.end();
}

}

#endif // STL_EXT_H
