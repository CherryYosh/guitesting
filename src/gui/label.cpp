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

Label::Label(TypeE t, Window* p, Control* c, LayerT l, float x, float y) : Control(t, p, c, l, x, y) {
	AddString("Testing");
}

Label::Label(Window* p, Control* c, LayerT l, float x, float y) : Control(LabelType, p, c, l, x, y) {
	AddString("Console");
}

Label::~Label() {
	//nothing
}
void Label::onMousePress(int button) {
	//needed?
}

void Label::onMouseRelease(int button) {
	//needed?
}

void Label::onKeyPress(unsigned short unicode) { }

void Label::onKeyRelease(int key, int mod) {
	//needed?
}

void Label::SetWidth(unsigned short w) {
	width = w;
}

void Label::SetHeight(unsigned short h) {
	height = h;
}

void Label::AddString(std::string s, util::Color c){
	text.push_back(colorstring(s, c));
}

std::vector<colorstring>& Label::GetText(){
	return text;
}
