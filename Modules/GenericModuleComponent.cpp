#include "GenericModuleComponent.h"

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
#include <chrono>
#include <sys/stat.h>
#include <debug/debug.h>

GenericModuleComponent::GenericModuleComponent(Entity& e, const std::string& dll_name)
    : Component(e) {
    // unsafe
    m_dll_handle = DLOPEN(dll_name.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!m_dll_handle) {
        report_error(DLERROR());
    }
    DLERROR();
    on_create_fn = (void (*)(C_Entity*))DLSYM(m_dll_handle, "on_create");
    if (!on_create_fn) {
        report("module {} did not implement {}", dll_name, "on_create");
        on_create_fn = [&](C_Entity*) {};
    }
    on_destroy_fn = (void (*)())DLSYM(m_dll_handle, "on_destroy");
    if (!on_destroy_fn) {
        report("module {} did not implement {}", dll_name, "on_destroy");
        on_destroy_fn = [&] {};
    }
    on_update_fn = (void (*)())DLSYM(m_dll_handle, "on_update");
    if (!on_update_fn) {
        report("module {} did not implement {}", dll_name, "on_update");
        on_update_fn = [&] {};
    }
    version_fn = (const char* (*)())DLSYM(m_dll_handle, "version");
    if (!version_fn) {
        report("module {} did not implement {}", dll_name, "version");
        static const char* default_version = "v0.0";
        version_fn = [] { return default_version; };
    }
    on_mouse_down_fn = (void (*)(C_vec_d))DLSYM(m_dll_handle, "on_mouse_down");
    if (!on_mouse_down_fn) {
        report("module {} did not implement {}", dll_name, "on_mouse_down");
        on_mouse_down_fn = [&](C_vec_d) {};
    }
    on_mouse_up_fn = (void (*)(C_vec_d))DLSYM(m_dll_handle, "on_mouse_up");
    if (!on_mouse_up_fn) {
        report("module {} did not implement {}", dll_name, "on_mouse_up");
        on_mouse_up_fn = [&](C_vec_d) {};
    }
    on_mouse_move_fn = (void (*)(C_vec_d))DLSYM(m_dll_handle, "on_mouse_move");
    if (!on_mouse_move_fn) {
        report("module {} did not implement {}", dll_name, "on_mouse_move");
        on_mouse_move_fn = [&](C_vec_d) {};
    }

    struct stat meta;
    auto ret = stat(dll_name.c_str(), &meta);
    if (ret != 0) {
        report_error("couldn't stat dynamic module file \"{}\": {}", dll_name, strerror(errno));
        return;
    }

    std::string last_changed = std::ctime(&meta.st_mtim.tv_sec);
    // ctime puts \n at the end, lets remove it
    last_changed.erase(last_changed.size() - 1);
    report(fmt::format("loaded dynamic module \"{}\" version {} (last changed: {})", dll_name, version_fn(), last_changed).c_str());

    on_create_fn(&parent());

    on_mouse_down = [&](GameWindow& window, const HID::MouseAction& action) {
        auto pos = action.world_position(window);
        on_mouse_down_fn({ pos.x, pos.y });
    };
    on_mouse_up = [&](GameWindow& window, const HID::MouseAction& action) {
        auto pos = action.world_position(window);
        on_mouse_up_fn({ pos.x, pos.y });
    };
    on_mouse_move = [&](GameWindow& window, const HID::MouseAction& action) {
        auto pos = action.world_position(window);
        on_mouse_move_fn({ pos.x, pos.y });
    };
    //o_info("hello");
}

GenericModuleComponent::~GenericModuleComponent() {
    on_destroy_fn();
    DLCLOSE(m_dll_handle);
}

void GenericModuleComponent::on_update() {
    on_update_fn();
}
