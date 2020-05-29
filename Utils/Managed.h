#ifndef MANAGED_H
#define MANAGED_H

#include <utility>

/// Owns a pointer and deletes it upon destruction
template<class T>
class Managed
{
    T* m_ptr { nullptr };

public:
    Managed(T*&& ptr)
        : m_ptr(std::move(ptr)) {
        ptr = nullptr;
    }
    Managed(const Managed&) = delete;
    Managed(Managed&& ptr) {
        delete m_ptr;
        m_ptr     = ptr.m_ptr;
        ptr.m_ptr = nullptr;
    }
    Managed& operator=(Managed&& ptr) {
        delete m_ptr;
        m_ptr     = ptr.m_ptr;
        ptr.m_ptr = nullptr;
    }

    ~Managed() {
        delete m_ptr;
    }

    static Managed adopt(T*& ptr) {
        Managed<T> managed(ptr);
        ptr = nullptr;
        return managed;
    }

    T*       operator->() { return m_ptr; }
    const T* operator->() const { return m_ptr; }

    T&       operator*() { return *m_ptr; }
    const T& operator*() const { return *m_ptr; }

    T*       get() { return m_ptr; }
    const T* get() const { return m_ptr; }
};

#endif // MANAGED_H
