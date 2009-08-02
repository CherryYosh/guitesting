/* 
 * File:   lua_utils.h
 * Author: brandon
 *
 * Created on August 1, 2009, 1:42 AM
 */

#ifndef _LUA_UTILS_H
#define	_LUA_UTILS_H

#include <lua.hpp>

void check_args(lua_State* L, int num){
    int numArgs = lua_gettop(L);
    if(numArgs != num){
	lua_error(L);
    }
}

void check_args_min(lua_State* L, int min){
    int numArgs = lua_gettop(L);
    if(numArgs < min){
	lua_error(L);
    }
}

void check_args_max(lua_State* L, int max){
    int numArgs = lua_gettop(L);
    if(numArgs > max){
	lua_error(L);
    }
}

void check_args_range(lua_State* L, int min, int max){
    int numArgs = lua_gettop(L);
    if(numArgs < min || numArgs > max){
	lua_error(L);
    }
}

#define __M_GET_USERDATA(type, data, index) type& data = *static_cast<type*> (L,luaL_checkudata(L,index,#type"_ud"));

#endif	/* _LUA_UTILS_H */

