#ifndef SCRIPTABLECOMPONENT_H
#define SCRIPTABLECOMPONENT_H

#include "Component.h"
#include "Entity.h"
#include "Core/Object.h"

#include <variant>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <functional>
#include "Utils/stl_ext.h"

class ScriptableComponent : public Component
{
    OBJNAME(ScriptableComponent)
private:
protected:
    std::string m_script_code;
    lua_State* m_lua_state;
    std::string m_scriptfile_name;
    bool m_script_initialized { false };
    std::vector<std::string> m_undefined_functions;

    void setup_globals();
    void initialize_script();

public:
    ScriptableComponent(Entity&, const std::string& scriptfile_name);
    ~ScriptableComponent();

    virtual void on_update() override;

    void register_global(int value, const std::string& name);
    void register_global(float value, const std::string& name);
    void register_global(const std::string& value, const std::string& name);
    void register_global(const luaL_Reg* value, const std::string& name);
    void register_global(std::uintptr_t value, const std::string& name);

    void load_global(const std::string& name) {
        lua_getglobal(m_lua_state, name.c_str());
    }

    void pop_stack() {
        lua_pop(m_lua_state, lua_gettop(m_lua_state));
    }
    
    void call_function(const std::string& name, int nargs, int nresults);

    typedef int (*LuaFunction)(lua_State*);

    void register_function(LuaFunction fn, const std::string& name);

    const std::string& scriptfile_name() const { return m_scriptfile_name; }
};

#endif // SCRIPTABLECOMPONENT_H
