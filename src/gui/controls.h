/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson
 */
/* A simple list of controls to be added... */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "control.h"
#include "window.h"
#include "button.h"
#include "rule.h"
#include "editbox.h"
#include "label.h"
#include "checkbox.h"
#include "slider.h"

#define __M_ControlCast(ptr,str)						\
	    if(str == "control"){ ptr = new Control(); }		\
	    else if(str == "button"){ ptr = new Button(); }		\
	    else if(str == "rule"){ ptr = new Rule(); }			\
	    else if(str == "editbox"){ ptr = new Editbox(); }		\
            else if(str == "label"){ ptr = new Label(); }               \
            else if(str == "checkbox"){ ptr = new Checkbox(); }         \
	    else { printf("%s not found!\n", str.c_str()); ptr = NULL; }
#endif
