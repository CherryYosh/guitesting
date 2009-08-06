#include "../gui/window.h"

#include "lua_window.h"
#include "lua_utils.h"

#include <string>
//================================== Table Methods

static int Window_Resizable(lua_State* L) {
    int numArgs = check_args_min(L, 1);
    __M_GET_USERDATA(Window, self, 1);

    if (numArgs == 1) { //Resizable() returns a bool
	lua_pushboolean(L, self.Resizable());
	return 1;
    } else { //Resizable(bool) we set it
	self.Resizable(lua_toboolean(L, 2));
	return 0;
    }
}

static int Window_SetBorders(lua_State* L) {
    check_args(L, 5);
    __M_GET_USERDATA(Window, self, 1);

    self.SetBorders(luaL_checkint(L, 2),
	    luaL_checkint(L, 3),
	    luaL_checkint(L, 4),
	    luaL_checkint(L, 5));

    return 0;
}

static int Window_CloseButton(lua_State* L) {
    check_args(L, 3);
    __M_GET_USERDATA(Window, self, 1);

    self.CloseButton(luaL_checkint(L, 2),
	    luaL_checkint(L, 3));
    return 0;
}

static int Window_NewChild(lua_State* L) {
    int args = check_args_range(L, 4, 5);
    __M_GET_USERDATA(Window, self, 1);

    Control* c = self.NewChild(luaL_checkstring(L, 2),
	    luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4),
	    (args == 5) ? (LayerT) luaL_checkint(L, 5) : DEFAULT_LAYER);

    __M_LUA_PUSH(c, "Control_ud");
    return 1;
}

//================================== Table Metatable
//================================== Methods
//================================== metatables
//================================== Tables

const luaL_reg Window_table_methods[] = {
    {NULL, NULL}
};

const luaL_reg Window_table_metatable[] = {
    {NULL, NULL}
};

const luaL_reg Window_methods[] = {
    {"Resizable", Window_Resizable},
    {"SetBorders", Window_SetBorders},
    {"CloseButton", Window_CloseButton},
    {"NewChild", Window_NewChild},
    {NULL, NULL}
};

const luaL_reg Window_metatable[] = {
    {NULL, NULL}
};

void Window_DoExtra(lua_State*) { }
