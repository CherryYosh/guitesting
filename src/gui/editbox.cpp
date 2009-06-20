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

#include "editbox.h"

Editbox::Editbox(Window* p, Control* c, LayerT l, float x, float y) : caretPos(0), currentLine(0), Label(EditboxType, p, c, l, x, y) {
    text.push_back("");
}

Editbox::Editbox(const Editbox& orig) : caretPos(orig.caretPos), currentLine(orig.currentLine), Label(orig) {
    text.push_back("");
}

Editbox::~Editbox() {
    //nothing right now
}

bool Editbox::HitTest(int mX, int mY) {
    if (mX > x && mX < x + width
	    && mY > y && mY < y + height) {
	return true;
    }
    return false;
}

void Editbox::OnMousePress(unsigned short button, int mX, int mY) {
}

void Editbox::OnKeyPress(unsigned short unicode, int key, int mod) {
    if (unicode > 31 && unicode < 126) { //alphabet
	text[currentLine].insert(caretPos, unicode);
	MoveCaret(1);
    } else if (unicode == 8) { //backspace
	if (0 < caretPos) {
	    text[currentLine].erase(caretPos - 1, 1);
	    MoveCaret(-1);
	} else if (0 < currentLine) {
	    MoveCaret(text[currentLine - 1].size());

	    text[currentLine - 1].append(text[currentLine]);

	    std::vector<colorstring>::iterator it = text.begin();
	    std::advance(it, currentLine);
	    text.erase(it);

	    ChangeCaretLine(currentLine - 1);
	}
    } else if (unicode == 13) { //enter
	if (currentLine == text.size() - 1) {
	    if (caretPos == text[currentLine].size()) {
		text.push_back("");
	    } else {
		text.push_back(text[currentLine].split(caretPos));
	    }
	} else {
	    std::vector<colorstring>::iterator it = text.begin();
	    std::advance(it, currentLine);

	    if (caretPos == text[currentLine].size()) {
		text.insert(it, "");
	    } else {
		text.insert(it, text[currentLine].split(caretPos));
	    }
	}

	ChangeCaretLine(currentLine + 1);
	SetCaretPos(0);
    }

    /* Arrow keys, unicode does not show these. */
    if (key == 273) { //up
	ChangeCaretLine(currentLine - 1);
    } else if (key == 274) { //down
	ChangeCaretLine(currentLine + 1);
    } else if (key == 275) { //right
	MoveCaret(1);
    } else if (key == 276) { //left
	MoveCaret(-1);
    }
}

void Editbox::ChangeCaretLine(int newLine) {
    if (newLine > -1 && newLine < text.size()) {
	currentLine = newLine;

	if (caretPos > text[currentLine].size()) {
	    caretPos = text[currentLine].size();
	}
    }
}

void Editbox::MoveCaret(int dir) {

    //FIX!!! cp = unsigned
    caretPos += dir;

    if (text[currentLine].size() < caretPos) {
	caretPos = text[currentLine].size();
    }
}

void Editbox::SetCaretPos(int pos) {
    caretPos = pos;

    if (text[currentLine].size() < caretPos) {
	caretPos = text[currentLine].size();
    } else if (caretPos < 0) {
	caretPos = 0;
    }
}

void Editbox::OnKeyRelease(int key, int mod) {
}

void Editbox::OnMouseRelease(int button) {
}
