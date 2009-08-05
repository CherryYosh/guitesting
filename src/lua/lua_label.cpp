#include "../gui/label.h"

#include "lua_label.h"
#include "lua_utils.h"

#include <string>
//================================== Table Methods

static int Label_multiline(lua_State* L){
	int args = check_args_min(L, 1);
	__M_GET_USERDATA(Label, self, 1);

	if(args == 1){ //multiline()
		lua_pushboolean(L, self.multiline());
		return 1;
	} else { //multiline(bool) we set it
		self.multiline(lua_toboolean(L, 2));
		return 0;
	}
}

//================================== Table Metatable
//================================== Methods
//================================== metatables
//================================== Tables

const luaL_reg Label_table_methods[] = {
	{NULL, NULL}
};

const luaL_reg Label_table_metatable[] = {
	{NULL, NULL}
};

const luaL_reg Label_methods[] = {
	{"multiline", Label_multiline},
	{NULL, NULL}
};

const luaL_reg Label_metatable[] = {
	{NULL, NULL}
};

void Label_DoExtra(lua_State* L){
}