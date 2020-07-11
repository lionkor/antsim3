#ifndef SCRIPTABLECOMPONENT_H
#define SCRIPTABLECOMPONENT_H

#include "Component.h"
#include "Entity.h"
#include "Core/Object.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

class ScriptableComponent : public Component
{
    OBJNAME(ScriptableComponent)
private:
    lua_State* m_lua_state;
    std::string m_script_code;
    
public:
    ScriptableComponent(Entity&, const std::string& scriptfile_name);
    ~ScriptableComponent();
    
    virtual void on_update() override;
};

#endif // SCRIPTABLECOMPONENT_H
