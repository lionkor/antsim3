#include "Utils/DebugTools.h"

#define INTERNAL
#include "GenericModule.h"

#include "Utils/DebugTools.h"

// C API implementation

static inline vecd to_vecd(C_vec_d v) {
    return { v.x, v.y };
}

static inline C_vec_d from_vecd(vecd v) {
    return { v.x, v.y };
}

#define MOD_ASSERT(x)                                                              \
    do {                                                                           \
        if (!(x)) {                                                                \
            report_error("module function {} assertion failed: {}", __func__, #x); \
        }                                                                          \
    } while (false)

extern "C" {
/// Gets the position of the entity in the world
C_vec_d get_position(C_Entity* c_entity) {
    auto pos = c_entity->transform().position();
    return { pos.x, pos.y };
}

/// gets the class name of the entity
size_t get_name(C_Entity* c_entity, char* buf) {
    MOD_ASSERT(c_entity != nullptr);
    MOD_ASSERT(buf != nullptr);
    std::strcpy(buf, c_entity->class_name().c_str());
    return c_entity->class_name().size();
}

/// Moves the entity by `delta` and returns the new position
C_vec_d move_by(C_Entity* c_entity, C_vec_d delta) {
    MOD_ASSERT(c_entity != nullptr);
    c_entity->transform().move_by({ delta.x, delta.y });
    return get_position(c_entity);
}

void o_info(const char* msg) {
    report("{}", msg);
}

void o_warning(const char* msg) {
    report_warning("{}", msg);
}

void o_error(const char* msg) {
    report_error("{}", msg);
}
}
