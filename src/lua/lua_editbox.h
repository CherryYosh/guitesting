/* 
 * File:   lua_editbox.h
 * Author: brandon
 *
 * Created on August 5, 2009, 2:45 AM
 */

#ifndef _LUA_EDITBOX_H
#define	_LUA_EDITBOX_H

#include <lua.hpp>

extern const luaL_reg Editbox_table_methods[];
extern const luaL_reg Editbox_table_metatable[];

extern const luaL_reg Editbox_methods[];
extern const luaL_reg Editbox_metatable[];

void Editbox_DoExtra(lua_State*);

#endif	/* _LUA_EDITBOX_H */

