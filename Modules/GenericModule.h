#ifndef GENERIC_MODULE_H
#define GENERIC_MODULE_H

/*
 * To be used for scripting/modding purposes. Implementations are in GenericModule.cpp.
 */


// engine API, implemented in GenericModuleComponent.cpp

#ifdef INTERNAL
#include "ECS/Entity.h"
typedef Entity C_Entity;
#else
typedef void C_Entity;
#endif // INTERNAL

#ifdef __cplusplus
#define EXPORT __attribute__((visibility("default")))
extern "C" {
#else
#define EXPORT
#endif

#include <stddef.h>

typedef struct {
    double x;
    double y;
} C_vec_d;

// public API
/// Gets the position of the entity in the world
EXPORT extern C_vec_d get_position(C_Entity*);
/// gets the class name of the entity
EXPORT extern size_t get_name(C_Entity*, char*);
/// Moves the entity by `delta` and returns the new position
EXPORT extern C_vec_d move_by(C_Entity*, C_vec_d delta);
/// Prints (o = output) a message as "INFO"
EXPORT extern void o_info(const char* msg);
/// Prints (o = output) a message as "WARNING"
EXPORT extern void o_warning(const char* msg);
/// Prints (o = output) a message as "ERROR"
EXPORT extern void o_error(const char* msg);

// override these
extern void on_create(C_Entity*);
extern void on_destroy();
extern void on_update();
extern const char* version();
extern void on_mouse_down(C_vec_d pos);
extern void on_mouse_up(C_vec_d pos);
extern void on_mouse_move(C_vec_d pos);

#ifdef __cplusplus
}
#endif

#endif // GENERIC_MODULE_H
