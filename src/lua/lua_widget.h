#ifndef _LUA_Widget_H
#define	_LUA_Widget_H

#include <lua.hpp>

extern const luaL_reg Widget_table_methods[];
extern const luaL_reg Widget_table_metatable[];

extern const luaL_reg Widget_methods[];
extern const luaL_reg Widget_metatable[];

void Widget_DoExtra(lua_State*);

#endif	