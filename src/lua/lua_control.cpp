#include "../gui/control.h"
#include "../gui/label.h"
#include "../gui/editbox.h"

#include "lua_control.h"
#include "lua_utils.h"

#include <string>
//================================== Table Methods

static int Control_SetWidth(lua_State* L){
	check_args(L, 2);
	__M_GET_USERDATA(Control, self, 1);

	if(lua_isnumber(L, 2)){
		self.SetWidth(luaL_checknumber(L, 2));
	} else {
		self.SetWidth(luaL_checkstring(L, 2));
	}
	return 0;
}

static int Control_SetOrientation(lua_State* L){
	check_args(L, 2);
	__M_GET_USERDATA(Control, self, 1);

	self.SetOrientation((OrientationT)luaL_checkint(L, 2));
	return 0;
}

static int Control_SetMovementFlags(lua_State* L){
	check_args(L, 2);
	__M_GET_USERDATA(Control, self, 1);

	self.SetMovementFlags(luaL_checkstring(L, 2));
	return 0;
}

static int Control_ToLabel(lua_State* L){
	check_args(L, 1);
	__M_GET_USERDATAPTR(Control, self, 1);

	void* mem = lua_newuserdata(L, sizeof(Label));
    luaL_getmetatable(L, "Label_ud");
    lua_setmetatable(L, -2);

	new (mem) Label(*self);
	return 1;
}

static int Control_ToEditbox(lua_State* L){
	check_args(L, 1);
	__M_GET_USERDATAPTR(Control, self, 1);

	void* mem = lua_newuserdata(L, sizeof(Editbox));
    luaL_getmetatable(L, "Editbox_ud");
    lua_setmetatable(L, -2);

	new (mem) Editbox(*self);
	return 1;
}

//================================== Table Metatable
//================================== Methods
//================================== metatables
//================================== Tables

const luaL_reg Control_table_methods[] = {
	{NULL, NULL}
};

const luaL_reg Control_table_metatable[] = {
	{NULL, NULL}
};

const luaL_reg Control_methods[] = {
	{"SetWidth", Control_SetWidth},
	{"SetOrientation", Control_SetOrientation},
	{"SetMovementFlags", Control_SetMovementFlags},
	{"ToLabel", Control_ToLabel},
	{"ToEditbox", Control_ToEditbox},
	{NULL, NULL}
};

const luaL_reg Control_metatable[] = {
	{NULL, NULL}
};

void Control_DoExtra(lua_State* L){
#define SET(v) lua_setglobal(L, #v); lua_pushinteger(L, v);

	SET(DontResize);
	SET(All);
	SET(Horizontal);
	SET(Vertical);
	SET(BOTTOM_LAYER);
	SET(DEFAULT_LAYER); 
	SET(TOP_LAYER);

	//lua_pop(L, lua_gettop(L));
	lua_settop(L, 0);
}