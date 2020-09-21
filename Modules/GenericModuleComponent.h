#ifndef GENERICMODULECOMPONENT_H
#define GENERICMODULECOMPONENT_H

#include "ECS/Component.h"

class GenericModuleComponent : public Component
{
    OBJNAME(Component)
private:
    void* m_dll_handle { nullptr };

protected:
    std::function<void()> on_create_fn { nullptr };
    std::function<void()> on_destroy_fn { nullptr };
    std::function<void()> on_update_fn { nullptr };

public:
    GenericModuleComponent(Entity& e, const std::string& dll_name);
    ~GenericModuleComponent();
    
    virtual void on_update() override;
};

#endif // GENERICMODULECOMPONENT_H
