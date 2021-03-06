/* 
 * File:   lua_utils.h
 * Author: brandon
 *
 * Created on August 1, 2009, 1:42 AM
 */

#ifndef _LUA_UTILS_H
#define	_LUA_UTILS_H

#include <lua.hpp>

void check_args(lua_State*, int);
int check_args_min(lua_State*, int);
int check_args_max(lua_State*, int);
int check_args_range(lua_State*, int, int);

void* check_data(lua_State*, void*);

#define __M_GET_USERDATA(type, data, index) type& data = *static_cast<type*> (check_data(L,luaL_checkudata(L,index,#type"_ud")));
#define __M_GET_USERDATAPTR(type, data, index) type* data = static_cast<type*> (check_data(L,luaL_checkudata(L,index,#type"_ud")));

#define __M_LUA_PUSH(data, tag) { \
    void** mem = static_cast<void**>(lua_newuserdata(L, sizeof(*mem)));\
    *mem = data;\
    luaL_getmetatable(L, tag);\
    lua_setmetatable(L, -2); }

#endif	/* _LUA_UTILS_H */

