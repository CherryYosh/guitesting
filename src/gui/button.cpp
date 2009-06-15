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

#include "button.h"
#include "window.h"

#include <iostream>

Button::Button() : Control() { }

Button::Button(const Button& orig) : Control(orig) { }

Button::Button(Window* p, Control* c, LayerT l, float x, float y) : Control(ButtonType, p, c, l, x, y) { }

Button::~Button() { }

Control* Button::clone() {
	return new Button(*this);
}

bool Button::HitTest(int mouseX, int mouseY) {
	if (mouseX > x && mouseX < x + width &&
		mouseY > y && mouseY < y + height) {
		return true;
	}
	return false;
}

void Button::OnMousePress(unsigned short button, int mx, int my) {
	StartEvent("onPress");
}

bool Button::OnMouseClick(unsigned short num, bool final) {
	//keep in mind, onClick events MUST call End() and remove their self from the roots event list
	StartEvent("onClick");
	return false; //we dont do anything
}
