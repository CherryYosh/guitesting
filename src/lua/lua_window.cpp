#include "../gui/window.h"

#include "lua_window.h"
#include "lua_utils.h"

#include <string>
//================================== Table Methods

static int Window_SetBorder(lua_State* L) {
    check_args(L, 5);
    __M_GET_USERDATA(Window, self, 1);

    self.SetBorder(luaL_checkint(L, 2),
	    luaL_checkint(L, 3),
	    luaL_checkint(L, 4),
	    luaL_checkint(L, 5));

    return 0;
}

static int Window_NewChild(lua_State* L) {
    int args = check_args_range(L, 4, 5);
    __M_GET_USERDATA(Window, self, 1);

    Widget* c;
    if(args == 4){
	c = self.NewChild(luaL_checkstring(L, 2),
		luaL_checknumber(L, 3),
		luaL_checknumber(L, 4));
    } else if(args == 5) {
	c = self.NewChild(luaL_checkstring(L, 2),
		luaL_checknumber(L, 3),
		luaL_checknumber(L, 4),
		(LayerT)luaL_checkint(L, 5));
    }

    __M_LUA_PUSH(c, "Widget_ud");
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
    {"SetBorder", Window_SetBorder},
    {"NewChild", Window_NewChild},
    {NULL, NULL}
};

const luaL_reg Window_metatable[] = {
    {NULL, NULL}
};

void Window_DoExtra(lua_State*) { }
