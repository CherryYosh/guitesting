#ifndef _LUA_WINDOW_H
#define	_LUA_WINDOW_H

#include <lua.hpp>

extern const luaL_reg Window_table_methods[];
extern const luaL_reg Window_table_metatable[];

extern const luaL_reg Window_methods[];
extern const luaL_reg Window_metatable[];

void Window_DoExtra(lua_State*);

#endif	