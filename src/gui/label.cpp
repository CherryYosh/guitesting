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
#include <GL/glew.h>
#include "label.h"

Label::Label(Window* p) : Widget(GUI_HAS_TEXT, p) {
}

Label::Label(long t, Window* p) : Widget(t, p) {
}

Label::Label(const Label& orig) : text(orig.text), isMultiline(orig.isMultiline), Widget(orig) {}

Label::~Label() {
	//nothing
}

Widget* Label::clone() {
	return new Label(*this);
}

void Label::AddString(colorstring s){
	this->text.push_back(s);
}

void Label::AddString(std::string s, util::Color c) {
	this->text.push_back(colorstring(s, c));
}

std::vector<colorstring>& Label::GetText() {
	return text;
}

void Label::multiline(bool v){
	isMultiline = v;
}

bool Label::multiline() {
	return isMultiline;
}
