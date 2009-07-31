#include "luabase.h"

#include <stdio.h>

lua_State* LUABase::L = NULL;
bool LUABase::initialized = false;

LUABase::LUABase() { }

LUABase::~LUABase() { }

void LUABase::Init() {
    if (!initialized) {
	L = lua_open();
	luaL_openlibs(L);
	initialized = true;
    }
}

lua_State* LUABase::GetLuaState() {
    return L;
}

bool LUABase::CallScript(std::string script, DebugLevel debug) {
    std::string d;
    std::string c;

    if (debug == None) {
	d = "nil";
    } else if (debug == Traceback) {
	d = "debug.traceback";
    } else if (debug == Interactive) {
	d = "debug.debug";
    }

    c = "local ok, errmsg = xpcall(function () dofile(\"" + script + "\") end, " + d + "); if not ok then print(errmsg) end";

    luaL_dostring(L, c.c_str());
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

    size_t size = args.size();
    for (unsigned int i = 0; i < size; i++) {
	if (__M_IsNumber(args[i])) {
	    lua_pushnumber(L, boost::any_cast<double>(args[i]));
	} else if (__M_IsString(args[i])) {
	    lua_pushstring(L, boost::any_cast<std::string > (args[i]).c_str());
	} else {
	}
    }

    /* do the call (size arguments, 0 result) */
    if (lua_pcall(L, size, 0, 0)) {
	printf("LuaError: error running script %s: %s\n", script.c_str(), lua_tostring(L, -1));
	return false;
    }

    return true;
}

