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

EXPORT C_vec_d get_position(C_Entity* entity) {
    ASSERT(entity);
    auto vec = entity->transform().position();
    C_vec_d result { vec.x, vec.y };
    return result;
}

EXPORT size_t get_name(C_Entity* entity, char* buffer) {
    ASSERT(entity);
    ASSERT(buffer);
    auto str = entity->class_name();
    std::strcpy(buffer, str.c_str());
    return str.size();
/// gets the class name of the entity
size_t get_name(C_Entity* c_entity, char* buf) {
    MOD_ASSERT(c_entity != nullptr);
    MOD_ASSERT(buf != nullptr);
    std::strcpy(buf, c_entity->class_name().c_str());
    return c_entity->class_name().size();
}

EXPORT C_vec_d move_by(C_Entity* entity, C_vec_d delta) {
    ASSERT(entity);
    entity->transform().move_by(to_vecd(delta));
    return from_vecd(entity->transform().position());
/// Moves the entity by `delta` and returns the new position
C_vec_d move_by(C_Entity* c_entity, C_vec_d delta) {
    MOD_ASSERT(c_entity != nullptr);
    c_entity->transform().move_by({ delta.x, delta.y });
    return get_position(c_entity);
}

EXPORT void o_info(const char* msg) {
    ASSERT(msg);
    report(msg);
void o_info(const char* msg) {
    report("{}", msg);
}

EXPORT void o_warning(const char* msg) {
    ASSERT(msg);
    report_warning(msg);
void o_warning(const char* msg) {
    report_warning("{}", msg);
}

EXPORT void o_error(const char* msg) {
    ASSERT(msg);
    report_warning(msg);
void o_error(const char* msg) {
    report_error("{}", msg);
}

}
