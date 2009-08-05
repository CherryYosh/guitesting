#ifndef _LUA_LABEL_H
#define	_LUA_LABEL_H

#include <lua.hpp>

extern const luaL_reg Label_table_methods[];
extern const luaL_reg Label_table_metatable[];

extern const luaL_reg Label_methods[];
extern const luaL_reg Label_metatable[];

void Label_DoExtra(lua_State*);

#endif	