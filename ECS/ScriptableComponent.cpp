#include "ScriptableComponent.h"

#include "Core/Application.h"
#include "Utils/stl_ext.h"
#include "Utils/Constants.h"

namespace LuaLib {

static Entity* get_entity(lua_State* L, long long i) {
    if (i == 0) {
        // we understand that we want the currently attached-to entity
        // which is stored in g_parent
        lua_getglobal(L, "g_parent");
        i = luaL_checkinteger(L, 1);
        lua_pop(L, 1);
    }
    return reinterpret_cast<Entity*>(i);
}

static Component* get_component(lua_State* L, long long i) {
    if (i == 0) {
        // we understand that we want the currently attached-to entity
        // which is stored in g_parent
        lua_getglobal(L, "g_this");
        i = luaL_checkinteger(L, 1);
        lua_pop(L, 1);
    }
    return reinterpret_cast<Component*>(i);
}

static void throw_error(lua_State* L, const std::string& message) {
    lua_pushstring(L, message.c_str());
    lua_error(L);
}

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
// void log_error(string)
static int log_error(lua_State* L) {
    const char* arg = luaL_checkstring(L, 1);
    lua_pop(L, 1);
    lua_getglobal(L, "g_scriptfile_name");
    const char* scriptname = lua_tostring(L, 1);
    impl::report_impl("{}{}[{}LUA{}{} ERROR] in {}: {}{}\n", ANSI_RESET, ANSI_RED, ANSI_BOLD, ANSI_RESET, ANSI_RED, scriptname, arg, ANSI_RESET);
    return 0;
}

// void log_warning(string)
static int log_warning(lua_State* L) {
    const char* arg = luaL_checkstring(L, 1);
    lua_pop(L, 1);
    lua_getglobal(L, "g_scriptfile_name");
    const char* scriptname = lua_tostring(L, 1);
    impl::report_impl("{}{}[{}LUA{}{} WARNING] in {}: {}{}\n", ANSI_RESET, ANSI_YELLOW, ANSI_BOLD, ANSI_RESET, ANSI_YELLOW, scriptname, arg, ANSI_RESET);
    return 0;
}

// void log_info(string)
static int log_info(lua_State* L) {
    const char* arg = luaL_checkstring(L, 1);
    lua_pop(L, 1);
    lua_getglobal(L, "g_scriptfile_name");
    const char* scriptname = lua_tostring(L, 1);
    impl::report_impl("[{}LUA{} INFO] in {}: {}\n", ANSI_BOLD, ANSI_RESET, scriptname, arg);
    return 0;
}

// number,number world_to_screen_pos(number, number)
static int world_to_screen_pos(lua_State* L) {
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
        double x = luaL_checknumber(L, 1);
        double y = luaL_checknumber(L, 2);
        lua_pop(L, 2);
        Component* comp = get_component(L, 0);
        auto vec = comp->window().mapCoordsToPixel(sf::Vector2f(x, y));
        lua_pushnumber(L, vec.x);
        lua_pushnumber(L, vec.y);
        return 2;
    } else {
        throw_error(L, "Engine.world_to_screen_pos expects two numbers as arguments");
    }
    return 0;
}

static const luaL_Reg g_engine_lib[] = {
    { "log_error", log_error },
    { "log_warning", log_warning },
    { "log_info", log_info },
    { "world_to_screen_pos", world_to_screen_pos },
};

}

namespace Vec {


// number,number normalize(number,number)
static int normalize(lua_State* L) {
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
        double x = luaL_checknumber(L, 1);
        double y = luaL_checknumber(L, 2);
        lua_pop(L, 2);
        vecd v(x, y);
        v.normalize();
        lua_pushnumber(L, v.x);
        lua_pushnumber(L, v.y);
        return 2;
    } else {
        throw_error(L, "Vec.normalize expects two numbers as arguments");
    }
    return 0;
}

// number length(number,number)
static int length(lua_State* L) {
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
        double x = luaL_checknumber(L, 1);
        double y = luaL_checknumber(L, 2);
        lua_pop(L, 2);
        vecd v(x, y);
        lua_pushnumber(L, v.length());
        return 1;
    } else {
        throw_error(L, "Vec.length expects two numbers as arguments");
    }
    return 0;
}

static const luaL_Reg g_vec_lib[] = {
    { "normalize", normalize },
    { "length", length },
};

}

namespace Entity {

// number,number position()
static int position(lua_State* L) {
    ::Entity* entity = get_entity(L, 0);
    lua_pushnumber(L, entity->transform().position().x);
    lua_pushnumber(L, entity->transform().position().y);
    return 2;
}

// number rotation()
static int rotation(lua_State* L) {
    ::Entity* entity = get_entity(L, 0);
    lua_pushnumber(L, entity->transform().rotation());
    return 1;
}

// void move_by(dx, dy)
static int move_by(lua_State* L) {
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
        double dx = luaL_checknumber(L, 1);
        double dy = luaL_checknumber(L, 2);
        lua_pop(L, 2);
        ::Entity* entity = get_entity(L, 0);
        entity->transform().move_by(vecd { dx, dy });
    } else {
        throw_error(L, "Entity.move_by expects two numbers as arguments");
    }
    return 0;
}

// void set_position(dx, dy)
static int set_position(lua_State* L) {
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
        double dx = luaL_checknumber(L, 1);
        double dy = luaL_checknumber(L, 2);
        lua_pop(L, 2);
        ::Entity* entity = get_entity(L, 0);
        entity->transform().set_position(vecd { dx, dy });
    } else {
        throw_error(L, "Entity.set_position expects two numbers as arguments");
    }
    return 0;
}

static const luaL_Reg g_entity_lib[] = {
    { "position", Entity::position },
    { "rotation", Entity::rotation },
    { "move_by", Entity::move_by },
    { "set_position", Entity::set_position },
};

}

static int error_handler(lua_State* L) {
    if (lua_isstring(L, 1)) {
        report_error("lua internal call error: {}", luaL_checkstring(L, 1));
    } else {
        report_error("error_handler didn't receive string");
    }
    return 0;
}

static inline int internal_pcall(lua_State* L, int nargs, int nret) {
    /* calculate stack position for message handler */
    int hpos = lua_gettop(L) - nargs;
    int ret = 0;
    /* push custom error message handler */
    lua_pushcfunction(L, LuaLib::error_handler);
    /* move it before function and arguments */
    lua_insert(L, hpos);
    /* call lua_pcall function with custom handler */
    ret = lua_pcall(L, nargs, nret, hpos);
    /* remove custom error message handler from stack */
    lua_remove(L, hpos);
    /* pass return value of lua_pcall */
    return ret;
}

static inline void dump_stack(lua_State* L) {
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) { /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

        case LUA_TSTRING: /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;

        case LUA_TBOOLEAN: /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;

        case LUA_TNUMBER: /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;

        default: /* other values */
            printf("%s", lua_typename(L, t));
            break;
        }
        printf("  "); /* put a separator */
    }
    printf("\n"); /* end the listing */
}

}

#define DUMP_STACK()  \
    report("STACK:"); \
    LuaLib::dump_stack(m_lua_state)

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
    register_global(reinterpret_cast<std::uintptr_t>(this), "g_this");

    setup_table_MouseButton();

    // setup global engine namespace function table
    register_global(LuaLib::Engine::g_engine_lib, "Engine");
    register_global(LuaLib::Entity::g_entity_lib, "Entity");
    register_global(LuaLib::Vec::g_vec_lib, "Vec");
}

void ScriptableComponent::initialize_script() {
    if (luaL_loadstring(m_lua_state, m_script_code.c_str()) == LUA_OK) {
        auto top = lua_gettop(m_lua_state);
        if (LuaLib::internal_pcall(m_lua_state, 0, 1) == LUA_OK) {
            lua_pop(m_lua_state, top);
        } else {
            report_error("lua_pcall failed: {}", luaL_checkstring(m_lua_state, 0));
            lua_pop(m_lua_state, 1);
            return;
        }
    } else {
        report_error("luaL_loadstring failed");
    }
    DUMP_STACK();
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
        if (LuaLib::internal_pcall(m_lua_state, nargs, nresults) != LUA_OK) {
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
        report("{}", pos);
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

    on_mouse_move = [&](GameWindow& window, const HID::MouseAction& ma) {
        auto pos = ma.world_position(window);
        report("{}", pos);
        load_global("on_mouse_move");
        lua_pushnumber(m_lua_state, pos.x);
        lua_pushnumber(m_lua_state, pos.y);
        call_function("on_mouse_move", 2, 0);
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
