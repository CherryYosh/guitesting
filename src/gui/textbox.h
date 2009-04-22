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
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "control.h"

#include <string>
#include <vector>

class Textbox : public Control {
public:
	Textbox( Window* p, std::string t, int x, int y );
	~Textbox();

	virtual void Render(); //we will do nothing here
	virtual void RenderText();

	virtual void onMousePress( int button );
	virtual void onMouseRelease( int button );

	virtual void onKeyPress( unsigned short unicode );
	virtual void onKeyRelease( int key, int mod );

	virtual void SetFont( unsigned char f );
	virtual void SetWidth( unsigned short );
	virtual void SetHeight( unsigned short );

//	static Shader* _TextShader;
protected:
	unsigned short TextWidth, TextHeight;
	unsigned short CaretPos, CaretLine;
	unsigned short BottomLine; //the index of the bottome line
	unsigned short NumCharacters, NumLines;
	unsigned short MaxCharacters, MaxLines;
	bool Multiline;
	bool FlashCaret;
	bool ShowingCaret;
	bool Editable;
	unsigned char font; 		//fixed at 0 right now
	std::vector<std::string> lines;
private:
};

#endif
