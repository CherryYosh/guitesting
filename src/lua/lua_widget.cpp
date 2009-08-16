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

static int Widget_SetHeight(lua_State* L) {
	check_args(L, 2);
	__M_GET_USERDATA(Widget, self, 1);

	if (lua_isnumber(L, 2)) {
		self.SetHeight(luaL_checknumber(L, 2));
	} else {
		self.SetHeight(luaL_checkstring(L, 2));
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

static int Widget_NewWidget(lua_State* L){
    int args = check_args_range(L, 2, 7);

    Widget* w = NULL;
    if(args == 2){
	w = Widget::NewWidget(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
    } else if(args == 3){
	w = Widget::NewWidget(luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_checkint(L, 3));
    }	else if(args == 4){
	w = Widget::NewWidget(luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
    }	else if(args == 5){
	w = Widget::NewWidget(luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4), LayerT(luaL_checkint(L, 5)));
    }	else if(args == 6){
	w = Widget::NewWidget(luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4), LayerT(luaL_checkint(L, 5)), ResizeConstraintT(luaL_checkint(L, 6)));
    }	else if(args == 7){
	w = Widget::NewWidget(luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4), LayerT(luaL_checkint(L, 5)), ResizeConstraintT(luaL_checkint(L, 6)), MovementConstrainT(luaL_checkint(L, 7)));
    }

    __M_LUA_PUSH(w, "Widget_ud");
    return 1;
}

//================================== Table Metatable
//================================== Methods
//================================== metatables
//================================== Tables

const luaL_reg Widget_table_methods[] = {
	{"NewWidget", Widget_NewWidget},
	{NULL, NULL}
};

const luaL_reg Widget_table_metatable[] = {
	{NULL, NULL}
};

const luaL_reg Widget_methods[] = {
	{"SetWidth", Widget_SetWidth},
	{"SetHeight", Widget_SetHeight},
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