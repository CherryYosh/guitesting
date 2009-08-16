#include "../gui/window.h"

#include "lua_window.h"
#include "lua_utils.h"

#include "../utils/color.h"
#include "../events/gui/close.h"
#include "../events/gui/colorchange.h"

#include <string>
//================================== Table Methods
//================================== Table Metatable
//================================== Methods
static int Window_SetBorder(lua_State* L) {
    check_args(L, 5);
    __M_GET_USERDATA(Window, self, 1);

    self.SetBorder(luaL_checkint(L, 2),
	    luaL_checkint(L, 3),
	    luaL_checkint(L, 4),
	    luaL_checkint(L, 5));

    return 0;
}

static int Window_AddChild(lua_State* L){
    check_args(L, 2);
    __M_GET_USERDATA(Window, self, 1);
    __M_GET_USERDATA(Widget, other, 2);

    self.AddChild(&other);
    return 0;
}

static int Window_FinishBorder(lua_State* L){
    check_args(L, 1);
    __M_GET_USERDATA(Window, self, 1);

    self.borderFinished = true;
    return 0;
}

static int Window_CloseButton(lua_State* L){
    int args = check_args_range(L, 1, 4);
    __M_GET_USERDATA(Window, self, 1);

    util::Color color("#FF000000"); //default to red
    float x = 0, y = 0;

    if(args > 1) x = luaL_checkint(L, 2);
    if(args > 2) y = luaL_checkint(L, 3);
    if(args > 3) color = util::Color(luaL_checkstring(L, 4));

    Widget* c = Widget::NewWidget("button", "closebutton", x, x, TOP_LAYER, RESIZE_NONE);

    ColorChangeEvent* ce = new ColorChangeEvent(c);

    ce->SetColor(color);

    c->AddEvent("onClick", new CloseEvent(c)); //OnClick we close
    c->AddEvent("onHover", ce); //onHover we turn to color

    self.AddChild(c);
    return 0;
}
//================================== metatables
//================================== Tables

const luaL_reg Window_table_methods[] = {
    {NULL, NULL}
};

const luaL_reg Window_table_metatable[] = {
    {NULL, NULL}
};

const luaL_reg Window_methods[] = {
    {"AddChild", Window_AddChild},
    {"SetBorder", Window_SetBorder},
    {"FinishBorder", Window_FinishBorder},
    {"CloseButton", Window_CloseButton},
    {NULL, NULL}
};

const luaL_reg Window_metatable[] = {
    {NULL, NULL}
};

void Window_DoExtra(lua_State*) { }
