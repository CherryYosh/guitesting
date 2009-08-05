#include "lua_utils.h"

void check_args(lua_State* L, int num){
    int numArgs = lua_gettop(L);
    if(numArgs != num){
		lua_error(L);
    }
}

int check_args_min(lua_State* L, int min){
    int numArgs = lua_gettop(L);
    if(numArgs < min){
		lua_error(L);
    }
	return numArgs;
}

int check_args_max(lua_State* L, int max){
    int numArgs = lua_gettop(L);
    if(numArgs > max){
		lua_error(L);
    }
	return numArgs;
}

int check_args_range(lua_State* L, int min, int max){
    int numArgs = lua_gettop(L);
    if(numArgs < min || numArgs > max){
		lua_error(L);
    }
	return numArgs;
}