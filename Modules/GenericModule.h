#ifndef GENERIC_MODULE_H
#define GENERIC_MODULE_H

#ifdef __cplusplus__
extern "C" {
#endif

// engine API, implemented in GenericModuleComponent.cpp

#ifdef INTERNAL
#include "ECS/Entity.h"
typedef Entity C_Entity;
#else
typedef void C_Entity;
#endif // INTERNAL

typedef struct {
    double x;
    double y;
} C_vec_d;

extern C_vec_d get_position(C_Entity*);

typedef void(*void_fn_t)();

// override these
extern void on_create();
extern void on_destroy();
extern void on_update();

#ifdef __cplusplus__
}
#endif

#endif // GENERIC_MODULE_H
