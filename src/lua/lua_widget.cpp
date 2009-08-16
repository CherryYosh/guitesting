#include "../gui/widget.h"
#include "../gui/label.h"
#include "../gui/editbox.h"

#include "lua_widget.h"
#include "lua_utils.h"

#include <string>
//================================== Table Methods

static int Widget_SetWidth(lua_State* L) {
	check_args(L, 2);
	__M_GET_USERDATA(Widget, self, 1);

	if (lua_isnumber(L, 2)) {
		self.SetWidth(luaL_checknumber(L, 2));
	} else {
		self.SetWidth(luaL_checkstring(L, 2));
	}
	return 0;
}

static int Widget_SetResizeConstraint(lua_State* L) {
	check_args(L, 2);
	__M_GET_USERDATA(Widget, self, 1);

	self.SetResizeConstraint(ResizeConstraintT(luaL_checkint(L, 2)));
	return 0;
}

static int Widget_ToLabel(lua_State* L) {
	check_args(L, 1);
	__M_GET_USERDATAPTR(Widget, self, 1);

	__M_LUA_PUSH((Label*)self, "Label_ud");
	return 1;
}

static int Widget_ToEditbox(lua_State* L) {
	check_args(L, 1);
	__M_GET_USERDATAPTR(Widget, self, 1);

	__M_LUA_PUSH((Editbox*)self, "Editbox_ud");
	return 1;
}

//================================== Table Metatable
//================================== Methods
//================================== metatables
//================================== Tables

const luaL_reg Widget_table_methods[] = {
	{NULL, NULL}
};

const luaL_reg Widget_table_metatable[] = {
	{NULL, NULL}
};

const luaL_reg Widget_methods[] = {
	{"SetWidth", Widget_SetWidth},
	{"SetResizeConstraint", Widget_SetResizeConstraint},
	{"ToLabel", Widget_ToLabel},
	{"ToEditbox", Widget_ToEditbox},
	{NULL, NULL}
};

const luaL_reg Widget_metatable[] = {
	{NULL, NULL}
};

void Widget_DoExtra(lua_State* L) {
#define SET(v) lua_pushinteger(L, v); lua_setglobal(L, #v);

	SET(RESIZE_NONE);
	SET(RESIZE_ALL);
	SET(RESIZE_HORIZONTAL);
	SET(RESIZE_VERTICAL);

	SET(BOTTOM_LAYER);
	SET(DEFAULT_LAYER);
	SET(TOP_LAYER);

	SET(GUI_NONE);
	SET(GUI_HAS_TEXT);
	SET(GUI_RECIEVE_KEYS);
	SET(GUI_RECIEVE_MOUSE);
	SET(GUI_CLICKABLE);
	SET(GUI_ALL);

	lua_settop(L, 0);
}