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
#include "../fontmgr.h"

#include <string>
#include <vector>
#include <list>

struct FontChar{
	char c;		//the actual char
	float r,g,b,a;	//colors
	float s,t,s2,t2;//texcoords
	float x,y;	//vertex position
	float width,height;	//width of the texture
	float advance;
};
struct FontString{
	unsigned short font;		//the font id
	unsigned short Size;		//the number of characters
	unsigned int Start; 		//the first position in the VBO
	unsigned int Length;		//the length (in bytes) we occupy in the vbo
	float y;			//the y coord wont change inside of a string
	unsigned int Width, Height;	//the current width of the string (used to find the x of the next char) and the height (needed?)
	std::list<FontChar> Text;
	unsigned int vao;
};

class Label : public Control {
public:
	Label( Window* p, std::string t, int x, int y );
	~Label();

	virtual void RenderText( int, int, int );

	virtual void onMousePress( int button );
	virtual void onMouseRelease( int button );

	virtual void onKeyPress( unsigned short unicode );
	virtual void onKeyRelease( int key, int mod );

	virtual void Move( float, float );

	virtual void SetWidth( unsigned short );
	virtual void SetHeight( unsigned short );

	virtual void UpdateVBO();
	virtual void AddStringsToVBO();
	virtual void ReplaceCharVBO( FontChar );
	virtual void RebuildVBOLine( FontString s );

	virtual	void AddChar( FontChar, bool );
	virtual void AppendString( unsigned int, unsigned int, FontString );
protected:
	unsigned short TextWidth;
	unsigned short CaretPos, CaretLine;
	unsigned short BottomLine; //the index of the bottome line
	unsigned short NumCharacters, NumLines;
	unsigned short MaxCharacters, MaxLines;
	unsigned int TextPosition, TextLength;
	bool Multiline;
	bool FlashCaret;
	bool ShowingCaret;
	bool Editable;
	std::vector<FontString> lines;
private:
};

#endif
