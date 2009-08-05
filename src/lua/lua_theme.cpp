/* 
 * File:   lua_theme.cpp
 * Author: brandon
 * 
 * Created on August 1, 2009, 1:40 AM
 */

#include "../theme.h"
#include "../gui/window.h"

#include "lua_theme.h"
#include "lua_utils.h"

#include <string>

//================================= table methods

static Theme* Theme_push(lua_State* L) {
    Theme* t = static_cast<Theme*>(lua_newuserdata(L, sizeof(Theme)));
    luaL_getmetatable(L, "Theme_ud");
    lua_setmetatable(L, -2);
    return t;
}

static int Theme_new(lua_State* L) {
    Theme_push(L);
    return 1;
}

static int Theme_SetImage(lua_State* L) {
    check_args(L, 1);

    std::string str = luaL_checkstring(L, 1);

    bool b = Theme::SetImage(str);

    lua_pushboolean(L, b);
    return 1;
}

//================================== methods

static int Theme_AddTextureData(lua_State* L) {
    check_args(L, 6);
    __M_GET_USERDATA(Theme, self, 1);

    self.AddTextureData(luaL_checkstring(L, 2),
	    luaL_checkint(L, 3),
	    luaL_checkint(L, 4),
	    luaL_checkint(L, 5),
	    luaL_checkint(L, 6));
    return 0;
}

static int Theme_Alias(lua_State* L){
    check_args(L, 3);
    __M_GET_USERDATA(Theme, self, 1);

    self.Alias(luaL_checkstring(L, 2), luaL_checkstring(L, 3));
    return 0;
}

static int Theme_NewWidget(lua_State* L){
    check_args_range(L, 2, 2); //should be 3 but im lazy right now
    __M_GET_USERDATA(Theme, self, 1);

    void* mem = lua_newuserdata(L, sizeof(Window));
    luaL_getmetatable(L, "Window_ud");
    lua_setmetatable(L, -2);

	//make sure that this works
	//This might cause a memory leak, but doing anything like delete w; causes a crash.
	Window* w = self.NewWidget(luaL_checkstring(L, 2));
	new (mem) Window(*w);

    return 1;
}

//================================== metamethods

static int Theme_tostring(lua_State* L) {
    check_args(L, 1);
    __M_GET_USERDATA(Theme, self, 1);

    lua_pushstring(L, "name");
    return 1;
}

//================================== tables

const luaL_reg Theme_table_methods[] = {
    {"new", Theme_new},
    {"SetImage", Theme_SetImage},
    {NULL, NULL}
};

const luaL_reg Theme_table_metatable[] = {
    {"__call", Theme_new},
    {NULL, NULL}
};

const luaL_reg Theme_methods[] = {
    {"AddTextureData", Theme_AddTextureData},
    {"Alias", Theme_Alias},
    {"NewWidget", Theme_NewWidget},
    {NULL, NULL}
};

const luaL_reg Theme_metatable[] = {
    {"__tostring", Theme_tostring},
    {NULL, NULL}
};

void Theme_DoExtra(lua_State*){};