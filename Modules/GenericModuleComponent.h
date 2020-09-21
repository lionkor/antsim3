#ifndef GENERICMODULECOMPONENT_H
#define GENERICMODULECOMPONENT_H

#include "ECS/Component.h"

#define INTERNAL
#include "GenericModule.h"

class GenericModuleComponent : public Component
{
    OBJNAME(Component)
private:
    void* m_dll_handle { nullptr };

protected:
    std::function<void(C_Entity*)> on_create_fn { nullptr };
    std::function<void()> on_destroy_fn { nullptr };
    std::function<void()> on_update_fn { nullptr };
    std::function<const char*()> version_fn { nullptr };
    std::function<void(C_vec_d)> on_mouse_down_fn { nullptr };
    std::function<void(C_vec_d)> on_mouse_up_fn { nullptr };
    std::function<void(C_vec_d)> on_mouse_move_fn { nullptr };

public:
    GenericModuleComponent(Entity& e, const std::string& dll_name);
    ~GenericModuleComponent();

    virtual void on_update() override;
};

#endif // GENERICMODULECOMPONENT_H
