#include "../gui/window.h"

#include "lua_window.h"
#include "lua_utils.h"

#include <string>
//================================== Table Methods

static int Window_SetBorder(lua_State* L) {
    //check_args(L, 5);
    __M_GET_USERDATA(Window, self, 1);

    self.SetBorder(luaL_checkint(L, 2),
	    luaL_checkint(L, 3),
	    luaL_checkint(L, 4),
	    luaL_checkint(L, 5));

    return 0;
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
    {NULL, NULL}
};

const luaL_reg Window_metatable[] = {
    {NULL, NULL}
};

void Window_DoExtra(lua_State*) { }
