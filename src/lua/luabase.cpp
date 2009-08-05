#include "luabase.h"

#include <cassert>
#include <stdio.h>
#include <iostream>

#include "lua_theme.h"
#include "lua_window.h"
#include "lua_control.h"
#include "lua_label.h"

//========================================================= Lua globals

static int lua_panic(lua_State *L) {
    lua_checkstack(L, 2);
    if (lua_type(L, -1) == LUA_TTABLE) {
	lua_rawgeti(L, -1, 2);
    }

    assert(false); //Fix this
    return 0;
}

//========================================================= Base Class

lua_State* LUABase::L = NULL;
bool LUABase::initialized = false;

LUABase::LUABase() { }

LUABase::~LUABase() { }

void LUABase::Init() {
    if (!initialized) {
	L = lua_open();
	lua_atpanic(L, lua_panic);

	luaL_openlibs(L);

/**
 * This creates the userdata, creating a global table of "name" from name_table_methods and name_table_metatable
 * a instance table of "name_ud" is created from name_methods and name_metatable tables
 * Any extra settings can be set via the name_DoExtra function
 */
#define REGISTER_USERDATA(name){\
	luaL_openlib(L, #name, name##_table_methods, 0);\
	luaL_newmetatable(L, #name);\
	luaL_register(L, NULL, name##_table_metatable);\
	lua_setmetatable(L, -2);\
	lua_newtable(L); \
	luaL_register(L, NULL, name##_methods); \
	luaL_newmetatable(L, #name"_ud" ); \
	luaL_register(L, NULL, name##_metatable ); \
	lua_pushliteral(L, "__index"); \
	lua_pushvalue(L, -3); \
	lua_rawset(L, -3); \
	lua_settop(L, 0); \
	name##_DoExtra(L); }

	REGISTER_USERDATA(Theme);
	REGISTER_USERDATA(Window);
	REGISTER_USERDATA(Control);
	REGISTER_USERDATA(Label);

	initialized = true;
    }
}

lua_State* LUABase::GetLuaState() {
    return L;
}

bool LUABase::CallScript(std::string script, DebugLevel debug) {
    return true;
}

bool LUABase::CallScript(std::string script, std::string arg1, DebugLevel debug) {
    if (luaL_dofile(L, script.c_str())) {
	printf("LuaError: %s\n", lua_tostring(L, -1));
	return false;
    }

    lua_getglobal(L, "main");
    if (!lua_isfunction(L, -1)) {
	printf("LuaError: function `main` not found, or value main not a global function.\n");

	lua_pop(L, 1);
	return false;
    }

    //push the first, and only, string arg
    lua_pushstring(L, arg1.c_str());

    /* do the call (1 arguments, 0 result) */
    if (lua_pcall(L, 1, 0, 0) != 0) {
	printf("LuaError: error running script %s: %s\n", script.c_str(), lua_tostring(L, -1));
	return false;
    }
    return true;
}

bool LUABase::CallScript(std::string script, LuaArgList args, DebugLevel debug) {
    return true;
}

