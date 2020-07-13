#include "ScriptableComponent.h"

#include "Core/Application.h"
#include "Utils/stl_ext.h"
#include "Utils/Constants.h"

namespace LuaLib {

void begin_table(lua_State* L) {
    lua_newtable(L);
}

void end_table(lua_State* L, const std::string& name) {
    lua_setglobal(L, name.c_str());
}

void begin_table_entry(lua_State* L, const std::string& name) {
    lua_pushstring(L, name.c_str());
}

void end_table_entry(lua_State* L) {
    lua_settable(L, -3);
}

namespace Engine {
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
    { "log_error", log_error },
    { "log_warning", log_warning },
    { "log_info", log_info },
};

}
namespace Entity {

static int position(lua_State* L) {

    lua_pushnumber(L, 5.0);
    lua_pushnumber(L, 6.0);
    return 2;
}

static const luaL_Reg g_entity_lib[] = {
    { "position", Entity::position },
};

}

}


void ScriptableComponent::setup_table_MouseButton() {
    begin_table();
    begin_table_entry("LMB");
    lua_pushinteger(m_lua_state, HID::MouseButton::Left);
    end_table_entry();
    begin_table_entry("RMB");
    lua_pushinteger(m_lua_state, HID::MouseButton::Right);
    end_table_entry();
    begin_table_entry("MMB");
    lua_pushinteger(m_lua_state, HID::MouseButton::Middle);
    end_table_entry();
    begin_table_entry("XB1");
    lua_pushinteger(m_lua_state, HID::MouseButton::Extra1);
    end_table_entry();
    begin_table_entry("XB2");
    lua_pushinteger(m_lua_state, HID::MouseButton::Extra2);
    end_table_entry();
    end_table("MouseButton");
}

void ScriptableComponent::setup_globals() {
    // sets up constant runtime globals
    register_global(m_scriptfile_name, "g_scriptfile_name");
    register_global(reinterpret_cast<std::uintptr_t>(&parent()), "g_parent");

    setup_table_MouseButton();

    // setup global engine namespace function table
    register_global(LuaLib::Engine::g_engine_lib, "Engine");
    register_global(LuaLib::Entity::g_entity_lib, "Entity");
}

void ScriptableComponent::initialize_script() {
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

void ScriptableComponent::call_function(const std::string& name, int nargs, int nresults) {
    static std::vector<std::string> undefined_functions;
    if (stl_ext::contains(undefined_functions, name)) {
        // already figured out that this function does not exist
        return;
    }
    if (lua_isnoneornil(m_lua_state, 1) == 1) {
        report_warning("'{}' does not exist in {}", name, m_scriptfile_name);
        undefined_functions.push_back(name);
    } else {
        if (lua_pcall(m_lua_state, nargs, nresults, 0) != LUA_OK) {
            report_error("calling '{}' failed in {}", name, m_scriptfile_name);
        }
    }
}

void ScriptableComponent::register_global(int value, const std::string& name) {
    lua_pushinteger(m_lua_state, value);
    lua_setglobal(m_lua_state, name.c_str());
}

void ScriptableComponent::register_global(std::uintptr_t value, const std::string& name) {
    lua_pushinteger(m_lua_state, static_cast<long long>(value));
    lua_setglobal(m_lua_state, name.c_str());
}

void ScriptableComponent::register_global(double value, const std::string& name) {
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

    auto& resman = resource_manager();
    auto maybe_lazyfile = resman.get_resource_by_name(scriptfile_name);
    if (maybe_lazyfile.error()) {
        report_error("ScriptableComponent failed to load script {}", scriptfile_name);
    } else {
        auto* data = maybe_lazyfile.value().get().load();
        m_script_code = std::string(data->begin(), data->end());
    }


    /*
    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_down { nullptr };
    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_up { nullptr };
    std::function<void(GameWindow&, const HID::MouseAction&)> on_mouse_move { nullptr };

    std::function<void(GameWindow&, const HID::Key&)> on_key_down { nullptr };
    std::function<void(GameWindow&, const HID::Key&)> on_key_up { nullptr };
    */

    on_mouse_down = [&](GameWindow& window, const HID::MouseAction& ma) {
        auto pos = ma.world_position(window);
        load_global("on_mouse_down");
        lua_pushinteger(m_lua_state, ma.button);
        lua_pushnumber(m_lua_state, pos.x);
        lua_pushnumber(m_lua_state, pos.y);
        call_function("on_mouse_down", 3, 0);
        pop_stack();
    };

    on_mouse_up = [&](GameWindow& window, const HID::MouseAction& ma) {
        auto pos = ma.world_position(window);
        load_global("on_mouse_up");
        lua_pushinteger(m_lua_state, ma.button);
        lua_pushnumber(m_lua_state, pos.x);
        lua_pushnumber(m_lua_state, pos.y);
        call_function("on_mouse_up", 3, 0);
        pop_stack();
    };

    setup_globals();
    initialize_script();
}

ScriptableComponent::~ScriptableComponent() {
    if (m_lua_state) {
        lua_close(m_lua_state);
    }
}

void ScriptableComponent::on_update() {
    load_global("update");
    call_function("update", 0, 0);
    pop_stack();
}

void ScriptableComponent::begin_table() {
    LuaLib::begin_table(m_lua_state);
}

void ScriptableComponent::end_table(const std::string& name) {
    LuaLib::end_table(m_lua_state, name);
}

void ScriptableComponent::begin_table_entry(const std::string& name) {
    LuaLib::begin_table_entry(m_lua_state, name);
}

void ScriptableComponent::end_table_entry() {
    LuaLib::end_table_entry(m_lua_state);
}
