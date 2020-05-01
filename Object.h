#ifndef OBJECT_H
#define OBJECT_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <sstream>

#include "DebugTools.h"

/*
 * Universal base class for all objects.
 * By default, Object is non-copyable.
 * An Object is identified uniquely by it's uuid.
 */

using UUID = boost::uuids::uuid;

static boost::uuids::random_generator s_uuid_gen;

class Object
{
    friend class CopyableObject;

private:
    UUID m_uuid;

    static inline UUID new_guid() {
        return s_uuid_gen();
    }

protected:
    Object()
        : m_uuid(new_guid()) {
    }

public:
    virtual ~Object() {
    }

    Object(const Object&) = delete;

    inline Object& operator=(const Object&) = delete;

    Object(Object&& other)
        : m_uuid(std::move(other.m_uuid)) {}

    inline Object& operator=(Object&& other) {
        m_uuid = std::move(other.m_uuid);
        return *this;
    }

    inline const UUID& uuid() const { return m_uuid; }

    virtual inline bool operator==(const Object& other) const { return m_uuid == other.m_uuid; }
    virtual inline bool operator!=(const Object& other) const { return !(*this == other); }

    // to be defined by the inheritor
    // use the SETNAME(...) macro for this!
    virtual std::string       class_name() const = 0;
    virtual std::stringstream to_stream() const {
        std::stringstream ss;
        ss << "uuid=" << boost::uuids::to_string(m_uuid) << ";";
        return ss;
    }

    friend std::ostream& operator<<(std::ostream&, const Object&);
};

inline std::ostream& operator<<(std::ostream& os, const Object& obj) {
    return os << "[" << (&obj)->class_name() << "]:{" << obj.to_stream().str() << "}";
}

class CopyableObject
    : public Object
{
protected:
    CopyableObject()
        : Object() {}

public:
    CopyableObject(const CopyableObject&) {}
    CopyableObject& operator=(const CopyableObject&) { return *this; }

    virtual inline bool operator==(const Object& other) const = 0;
    virtual inline bool operator!=(const Object& other) const = 0;
};

#define OBJECT(classname)                                    \
public:                                                      \
    virtual inline std::string class_name() const override { \
        return std::string(#classname);                      \
    }

#endif // OBJECT_H
