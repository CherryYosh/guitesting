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

#define __M_GET_USERDATA(type, data, index) type& data = *static_cast<type*> (L,luaL_checkudata(L,index,#type"_ud"));
#define __M_GET_USERDATAPTR(type, data, index) type* data = static_cast<type*> (L,luaL_checkudata(L,index,#type"_ud"));

#endif	/* _LUA_UTILS_H */

