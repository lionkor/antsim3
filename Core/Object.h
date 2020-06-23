// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef OBJNAME_H
#define OBJNAME_H

#include <string>
#include <sstream>
#include <functional>
#include <atomic>

#define SHOW_UUID 0

#include "Utils/DebugTools.h"

/*
 * Universal base class for all objects.
 * By default, Object is non-copyable.
 * An Object is identified uniquely by it's uuid.
 */

using UUID = size_t;

static inline std::atomic_size_t s_uuid_counter = 0;

class Object
{
private:
    UUID m_uuid;

    static inline UUID new_uuid() {
        return ++s_uuid_counter;
    }

protected:
    Object()
        : m_uuid(new_uuid()) {
    }

public:
    Object(const Object&)
        : Object() { }

    inline Object& operator=(const Object&) {
        m_uuid = new_uuid();
        return *this;
    }

    Object(Object&& other)
        : m_uuid(std::move(other.m_uuid)) { }

    inline Object& operator=(Object&& other) {
        m_uuid = std::move(other.m_uuid);
        return *this;
    }

    virtual ~Object() { }

    virtual const UUID& uuid() const final { return m_uuid; }

    virtual bool operator==(const Object& other) const { return m_uuid == other.m_uuid; }
    virtual bool operator!=(const Object& other) const { return !(*this == other); }

    // to be defined by the inheritor
    // use the OBJNAME(...) macro for this!
    virtual std::string       class_name() const = 0;
    virtual std::stringstream to_stream() const {
        std::stringstream ss;
#if SHOW_UUID
        ss << "uuid=" << m_uuid << ";";
#endif // SHOW_UUID
        return ss;
    }

    friend std::ostream& operator<<(std::ostream&, const Object&);
};

inline std::ostream& operator<<(std::ostream& os, const Object& obj) {
    return os << "[" << (&obj)->class_name() << "]:{" << obj.to_stream().str() << "}";
}

#define OBJNAME(classname)                            \
public:                                               \
    static std::string class_name_static() {          \
        return std::string(#classname);               \
    }                                                 \
    virtual std::string class_name() const override { \
        return classname::class_name_static();        \
    }

#define TS_BEGIN(base_class) auto ss = base_class::to_stream()
#define TS_PROP(property) ss << #property << "=" << property << ";"
#define TS_PROP_S(name, property) ss << name << "=" << property << ";"
#define TS_PROP_M(property)                                        \
    do {                                                           \
        auto __name = std::string(#property);                      \
        __name.erase(0, 2);                                        \
        auto __res = std::find(__name.begin(), __name.end(), '('); \
        if (__res != __name.end())                                 \
            __name.erase(__res);                                   \
        __res = std::find(__name.begin(), __name.end(), ')');      \
        if (__res != __name.end())                                 \
            __name.erase(__res);                                   \
        ss << __name << "=" << property << ";";                    \
    } while (false)

#define TS_END() return ss

#endif // OBJNAME_H
