#include "ScriptableComponent.h"

#include "Core/Application.h"

ScriptableComponent::ScriptableComponent(Entity& e, const std::string& scriptfile_name)
    : Component(e)
    , m_lua_state(luaL_newstate()) {
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
}

ScriptableComponent::~ScriptableComponent() {
    if (m_lua_state) {
        lua_close(m_lua_state);
    }
}

void ScriptableComponent::on_update() {
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
