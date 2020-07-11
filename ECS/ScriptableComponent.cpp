#include "ScriptableComponent.h"

#include "Core/Application.h"

namespace LuaLib {

static int log_error(lua_State* L) {
    const char* arg = luaL_checkstring(L, 1);
    lua_pop(L, 1);
    lua_getglobal(L, "g_scriptfile_name");
    const char* scriptname = lua_tostring(L, 1);
    impl::report_impl("{}{}[{}LUA{}{} ERROR] in {}: {}{}\n", ANSI_RESET, ANSI_RED, ANSI_BOLD, ANSI_RESET, ANSI_RED, scriptname, arg, ANSI_RESET);
    return 0;
}

static int log_warning(lua_State* L) {
    const char* arg = luaL_checkstring(L, 1);
    lua_pop(L, 1);
    lua_getglobal(L, "g_scriptfile_name");
    const char* scriptname = lua_tostring(L, 1);
    impl::report_impl("{}{}[{}LUA{}{} WARNING] in {}: {}{}\n", ANSI_RESET, ANSI_YELLOW, ANSI_BOLD, ANSI_RESET, ANSI_YELLOW, scriptname, arg, ANSI_RESET);
    return 0;
}

static int log_info(lua_State* L) {
    const char* arg = luaL_checkstring(L, 1);
    lua_pop(L, 1);
    lua_getglobal(L, "g_scriptfile_name");
    const char* scriptname = lua_tostring(L, 1);
    impl::report_impl("[{}LUA{} INFO] in {}: {}\n", ANSI_BOLD, ANSI_RESET, scriptname, arg);
    return 0;
}

static const luaL_Reg g_engine_lib[] = {
    { "log_error", LuaLib::log_error },
    { "log_warning", LuaLib::log_warning },
    { "log_info", LuaLib::log_info },
};

}

void ScriptableComponent::setup_globals() {
    // sets up constant runtime globals
    register_global(m_scriptfile_name, "g_scriptfile_name");
    
    // setup global engine namespace function table
    register_global(LuaLib::g_engine_lib, "Engine");
}

void ScriptableComponent::run_script() {
    if (luaL_loadstring(m_lua_state, m_script_code.c_str()) == LUA_OK) {
        auto top = lua_gettop(m_lua_state);
        if (lua_pcall(m_lua_state, 0, 1, 0) == LUA_OK) {
            lua_pop(m_lua_state, top);
        } else {
            report_error("lua_pcall failed");
        }
    } else {
        report_error("luaL_loadstring failed");
    }
}

void ScriptableComponent::register_global(const luaL_Reg* value, const std::string& name) {
    lua_newtable(m_lua_state);
    luaL_setfuncs(m_lua_state, value, 0);
    lua_setglobal(m_lua_state, name.c_str());
}

void ScriptableComponent::register_global(int value, const std::string& name) {
    lua_pushinteger(m_lua_state, value);
    lua_setglobal(m_lua_state, name.c_str());
}

void ScriptableComponent::register_global(float value, const std::string& name) {
    lua_pushnumber(m_lua_state, value);
    lua_setglobal(m_lua_state, name.c_str());
}

void ScriptableComponent::register_global(const std::string& value, const std::string& name) {
    lua_pushstring(m_lua_state, value.c_str());
    lua_setglobal(m_lua_state, name.c_str());
}

void ScriptableComponent::register_function(ScriptableComponent::LuaFunction fn, const std::string& name) {
    lua_pushcfunction(m_lua_state, fn);
    lua_setglobal(m_lua_state, name.c_str());
}

ScriptableComponent::ScriptableComponent(Entity& e, const std::string& scriptfile_name)
    : Component(e)
    , m_lua_state(luaL_newstate())
    , m_scriptfile_name(scriptfile_name) {
    // load lua standard libraries
    luaL_openlibs(m_lua_state);

    auto& resman = parent().world().application().resource_manager();
    auto maybe_lazyfile = resman.get_resource_by_name(scriptfile_name);
    if (maybe_lazyfile.error()) {
        report_error("ScriptableComponent failed to load script {}", scriptfile_name);
    } else {
        auto* data = maybe_lazyfile.value().get().load();
        m_script_code = std::string(data->begin(), data->end());
    }

    setup_globals();
}

ScriptableComponent::~ScriptableComponent() {
    if (m_lua_state) {
        lua_close(m_lua_state);
    }
}

void ScriptableComponent::on_update() {
    run_script();
}
