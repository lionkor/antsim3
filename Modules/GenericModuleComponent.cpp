#include "GenericModuleComponent.h"
#define INTERNAL
#include "GenericModule.h"
#ifdef __linux__
#include <dlfcn.h>
#define DLOPEN dlopen
#define DLSYM dlsym
#define DLCLOSE dlclose
#define DLERROR dlerror
#else
#error "currently not supported, please implement"
#endif

#include <functional>


GenericModuleComponent::GenericModuleComponent(Entity& e, const std::string& dll_name)
    : Component(e) {
    // unsafe
    m_dll_handle = DLOPEN(dll_name.c_str(), RTLD_LAZY);
    if (!m_dll_handle) {
        report_error(DLERROR());
    }
    DLERROR();
    on_create_fn = (void_fn_t)DLSYM(m_dll_handle, "on_create");
    on_destroy_fn = (void_fn_t)DLSYM(m_dll_handle, "on_destroy");
    on_update_fn = (void_fn_t)DLSYM(m_dll_handle, "on_update");

    on_create_fn();
}

GenericModuleComponent::~GenericModuleComponent() {
    on_destroy_fn();
    DLCLOSE(m_dll_handle);
}

void GenericModuleComponent::on_update() {
    on_update_fn();
}

// C API implementation

C_vec_d get_position(C_Entity* entity) {
    ASSERT(entity);
    auto vec = entity->transform().position();
    C_vec_d result { vec.x, vec.y };
    return result;
}
