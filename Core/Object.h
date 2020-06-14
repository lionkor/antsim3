#ifndef OBJNAME_H
#define OBJNAME_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <sstream>
#include <functional>

#define SHOW_UUID 0

#include "Utils/DebugTools.h"

/*
 * Universal base class for all objects.
 * By default, Object is non-copyable.
 * An Object is identified uniquely by it's uuid.
 */

using UUID = boost::uuids::uuid;

static boost::uuids::random_generator s_uuid_gen;

class Object
{
private:
    UUID m_uuid;

    static inline UUID new_uuid() {
        return s_uuid_gen();
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
        ss << "uuid=" << boost::uuids::to_string(m_uuid) << ";";
#endif // SHOW_UUID
        return ss;
    }

    friend std::ostream& operator<<(std::ostream&, const Object&);
};

inline std::ostream& operator<<(std::ostream& os, const Object& obj) {
    return os << "[" << (&obj)->class_name() << "]:{" << obj.to_stream().str() << "}";
}

#define OBJNAME(classname)                             \
public:                                               \
    static std::string class_name_static() {          \
        return std::string(#classname);               \
    }                                                 \
    virtual std::string class_name() const override { \
        return classname::class_name_static();        \
    }


#endif // OBJNAME_H
