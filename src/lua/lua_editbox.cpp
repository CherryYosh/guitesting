/* 
 * File:   lua_editbox.cpp
 * Author: brandon
 * 
 * Created on August 5, 2009, 2:45 AM
 */

#include "../gui/editbox.h"

#include "lua_editbox.h"
#include "lua_utils.h"

#include <string>

//================================= table methods

static int Editbox_SetDialog(lua_State* L){
    check_args(L, 2);
    __M_GET_USERDATA(Editbox, self, 1);

    Label* l = static_cast<Label*>(luaL_checkudata(L, 2, "Editbox_ud"));
    self.SetDialog(l);
    return 0;
}
//================================== methods
//================================== metamethods
//================================== tables

const luaL_reg Editbox_table_methods[] = {
    {NULL, NULL}
};

const luaL_reg Editbox_table_metatable[] = {
    {NULL, NULL}
};

const luaL_reg Editbox_methods[] = {
    {"SetDialog", Editbox_SetDialog},
    {NULL, NULL}
};

const luaL_reg Editbox_metatable[] = {
    {NULL, NULL}
};

void Editbox_DoExtra(lua_State*){};

