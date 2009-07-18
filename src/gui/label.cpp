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
#include "../fontmgr.h"

Label::Label(long t, Window* p, Control* c, LayerT l, float x, float y) : Control(t, p, c, l, x, y) {
}

Label::Label(Window* p, Control* c, LayerT l, float x, float y) : Control(GUI_HAS_TEXT, p, c, l, x, y) {
}

Label::Label(const Label& orig) : text(orig.text), isMultiline(orig.isMultiline), Control(orig) {}

Label::~Label() {
    //nothing
}

Control* Label::clone() {
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
