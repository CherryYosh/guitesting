#include "luabase.h"

#include <stdio.h>

static lua_State* L;
static bool initialized = false;

extern "C" {
	extern int luaopen_test(lua_State* S);
}

LUABase::LUABase() { }

LUABase::~LUABase() { }

void LUABase::Init() {
	if (initialized)
		return;
	initialized = true;

	L = lua_open();
	luaL_openlibs(L);
	luaopen_test(L);
	//luabind::open(L);

	printf("%i\n", lua_checkstack(L, 50));
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

	luaL_dostring(GetLuaState(), c.c_str());
	return true;
}

bool LUABase::CallScriptS(std::string script, std::string arg1, DebugLevel debug) {
	if (luaL_dofile(L, script.c_str())) {
		printf("LuaError: %s\n", lua_tostring(L, -1));
		return false;
	}

	lua_getglobal(L, "main");
	if (!lua_isfunction(L, -1)) {
		printf("LuaError: function `main` not found!!\n");

		lua_pop(L, 1);
		return false;
	}

	//push the first, and only, string arg
	lua_pushstring(L, arg1.c_str());

	/* do the call (1 arguments, 0 result) */
	if (lua_pcall(L, 1, 0, 0) != 0) {
		printf("LuaError: error running script %s: %s\n",script.c_str(), lua_tostring(L, -1));
		return false;
	}

	return true;
}

