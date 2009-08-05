#ifndef _LUA_CONTROL_H
#define	_LUA_CONTROL_H

#include <lua.hpp>

extern const luaL_reg Control_table_methods[];
extern const luaL_reg Control_table_metatable[];

extern const luaL_reg Control_methods[];
extern const luaL_reg Control_metatable[];

void Control_DoExtra(lua_State*);

#endif	