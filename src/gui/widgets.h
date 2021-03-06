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
/* A simple list of Widgets to be added... */

#ifndef WIDGETS_H
#define WIDGETS_H

#include "widget.h"
#include "window.h"
#include "button.h"
#include "rule.h"
#include "editbox.h"
#include "label.h"
#include "edge.h"

#define __M_WIDGETCAST(ptr,str)					\
	if(str == "widget"){ ptr = new Widget(); }		\
		else if(str == "button"){ ptr = new Button(); }		\
		else if(str == "rule"){ ptr = new Rule(); }			\
		else if(str == "editbox"){ ptr = new Editbox(); }		\
			else if(str == "label"){ ptr = new Label(); }               \
			else if(str == "edge"){ ptr = new Edge(); }         \
		else { throw std::invalid_argument("Widgets::__M_WidgetCast::" + str + "_not_valid_widget" ); }
#endif
