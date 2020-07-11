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

#include <functional>

class ScriptableComponent : public Component
{
    OBJNAME(ScriptableComponent)
private:
protected:
    std::string m_script_code;
    lua_State* m_lua_state;
    std::string m_scriptfile_name;

    void setup_globals();
    void run_script();

public:
    ScriptableComponent(Entity&, const std::string& scriptfile_name);
    ~ScriptableComponent();

    virtual void on_update() override;

    void register_global(int value, const std::string& name);
    void register_global(float value, const std::string& name);
    void register_global(const std::string& value, const std::string& name);
    void register_global(const luaL_Reg* value, const std::string& name);

    typedef int (*LuaFunction)(lua_State*);

    void register_function(LuaFunction fn, const std::string& name);

    const std::string& scriptfile_name() const { return m_scriptfile_name; }
};

#endif // SCRIPTABLECOMPONENT_H
