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
#ifndef EDITBOX_H
#define EDITBOX_H

#include "label.h"

class Editbox : public Label {
public:
	Editbox(Window* = NULL);
	Editbox(const Editbox& orig);
	~Editbox();

	Widget* clone();

	void OnKeyPress(unsigned short, int, int);

	void ChangeCaretLine(int);
	void MoveCaret(int);
	void SetCaretPos(int);

	void AddString(std::string , util::Color = "#FFFFFF");

	void SetDialog(Label*);
protected:
	Label* peer;

	int caretPos;
	int currentLine;
};

#endif
