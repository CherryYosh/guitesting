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
#ifndef BUTTON_H
#define BUTTON_H

#include "control.h"

class Button : public Control {
public:
	Button();
	Button(const Button&);
	Button(Window*, Control* = NULL, LayerT = DEFAULT_LAYER, float = 0, float = 0);
	~Button();

	Control* clone();
protected:
private:
};

#endif
