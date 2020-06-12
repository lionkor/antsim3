#ifndef MANAGED_H
#define MANAGED_H

#include <utility>

#include <memory>

template<typename... Args>
using Managed = std::unique_ptr<Args...>;

#define make_managed std::make_unique;

template<typename... Args>
using SharedPtr = std::shared_ptr<Args...>;

using std::make_shared;

template<typename... Args>
using WeakPtr = std::weak_ptr<Args...>;

/*
/// Owns a pointer and deletes it upon destruction
template<class T>
class Managed
{
    T* m_ptr { nullptr };

public:
    Managed(T*&& ptr)
        : m_ptr(std::move(ptr)) {
        // cleanup moved-from
        ptr = nullptr;
    }
    Managed(Managed&& ptr) {
        // delete held pointer
        delete m_ptr;
        // take new pointer
        m_ptr = ptr.m_ptr;
        // cleanup the moved-from object
        ptr.m_ptr = nullptr;
    }
    Managed& operator=(Managed&& ptr) {
        // delete held pointer
        delete m_ptr;
        // take new pointer
        m_ptr = ptr.m_ptr;
        // cleanup the moved-from object
        ptr.m_ptr = nullptr;
        return *this;
    }

    Managed(const Managed&) = delete;
    Managed& operator=(const Managed&) = delete;

    ~Managed() {
        delete m_ptr;
    }

    T*       operator->() { return m_ptr; }
    const T* operator->() const { return m_ptr; }

    T&       operator*() { return *m_ptr; }
    const T& operator*() const { return *m_ptr; }

    T*       get() { return m_ptr; }
    const T* get() const { return m_ptr; }
};
*/

#endif // MANAGED_H
