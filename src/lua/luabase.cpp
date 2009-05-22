#include "luabase.h"

#include <stdio.h>

static lua_State* L;
static bool initialized = false;

extern "C" {
	extern int luaopen_test(lua_State* S);
}

LUABase::LUABase(){
}

LUABase::~LUABase(){

}

void LUABase::Init(){
	if(initialized)
		return;
	initialized = true;

	L = lua_open();
	luaL_openlibs(L);
	luaopen_test(L);
	//luabind::open(L);

	printf("%i\n",lua_checkstack(L, 50));
}

lua_State* LUABase::GetLuaState(){
	return L;
}

bool LUABase::CallScript(std::string script, DebugLevel debug){
	std::string d;
	std::string c;

	if(debug == None){
		d = "nil";
	} else if (debug == Traceback){
		d = "debug.traceback";
	} else if (debug == Interactive){
		d = "debug.debug";
	}

	c = "local ok, errmsg = xpcall(function () dofile(\"" + script + "\") end, " + d + "); if not ok then print(errmsg) end";

	luaL_dostring(GetLuaState(), c.c_str());
	return true;
}

