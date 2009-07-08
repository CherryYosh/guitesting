/* 
 * File:   checkbox.cpp
 * Author: brandon
 * 
 * Created on May 2, 2009, 5:23 PM
 */

#include "checkbox.h"

Checkbox::Checkbox(Window* p, Control* c, LayerT l, float x, float y) : Control(CheckboxType, p, c, l, x, y) {
	Control* c2 = new Control(CheckboxType, p, this);
	c2->SetColor(0.0, 0.0, 0.0, -1.0); //make it invis, need a better way
	children.push_back(c2);

	isChecked = false;
}

Checkbox::~Checkbox() { }

bool Checkbox::OnMouseClick(unsigned short, bool) {
	isChecked = !isChecked;
	children[0]->SetColor(0.0, 0.0, 0.0, -1.0 + isChecked);

	return true;
}

