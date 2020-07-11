#include "ScriptableComponent.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

ScriptableComponent::ScriptableComponent(Entity& e)
    : Component(e) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Our Lua code, it simply prints a Hello, World message
    const char* code = "print('Hello, World')";

    // Here we load the string and use lua_pcall for run the code
    if (luaL_loadstring(L, code) == LUA_OK) {
        if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
            // If it was executed successfuly we
            // remove the code from the stack
            lua_pop(L, lua_gettop(L));
        }
    }

    lua_close(L);
}
