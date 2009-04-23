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
#include "../fontmgr.h"


Editbox::Editbox( Window* p, std::string t, int x, int y ) : Label( p, t, x, y ){
	FlashCaret = true;
	Editable = true;
	Multiline = true;
}

Editbox::~Editbox(){
	//nothing right now
}

bool Editbox::HitTest( int mX, int mY ){
	if( mX > x && mX < x + Width
			&& mY > y && mY < y + Height ){
		return true;
	}
	return false;
}

void Editbox::OnMousePress( unsigned short button, int mX, int mY ){
	if( button == 0 ){
		//tell if they clicked a line
		short size = lines.size();
		short height = FontMgr_GetLineHeight( font );
		FontString line;

		for( unsigned short i = 0; i < size; i++ ){
			//first a check to get the line
			if( mY < y + ( height * ( TextHeight - i ) ) ){
				CaretLine = i;
				line = lines[i];

				std::list<FontChar>::iterator it;
				unsigned int j = line.Text.size();
				unsigned int width = line.Width;
				for( it=line.Text.end(); it != line.Text.begin(); it--){
					if( mX > x + width ){
						CaretPos = j;
						return;
					}

					width -= ((FontChar)*it).advance;
					j--;
				}

				//so if we are here its in the line, but not the text sooo....
				CaretPos = line.Size;
				return;
			}
		}
	}
}

void Editbox::OnKeyPress( unsigned short unicode ){
	FontString* line = &lines[CaretLine];
	if( unicode > 31 && unicode < 126 && NumCharacters < MaxCharacters){

		//WARNING: Error might occure with the wunicode
		FontChar c;
		c.c = unicode;
		c.r = 0.0;
		c.g = 0.0;
		c.b = 1.0;
		c.a = 1.0;
		char p = 0;//char p = ( CaretPos > 0 ) ? (*--line->Text.end()).c : 0;
		FontMgr_GetCharData( line->font, p, c );

		AddChar( c, true );
		return;
	}

	if( unicode == 8 ){ //backspace
		if( CaretPos > 0){ //just remove one char
			std::list<FontChar>::iterator it = line->Text.begin();
			std::advance( it, --CaretPos );
			line->Width -= (*it).advance;
			line->Text.erase( it );
			NumCharacters--;
			return;
		} else if( Multiline && CaretLine > 0 ){
			//backspaced pressed at the end of the line..
			//all this does and delet the current line, moving its
			//content to the new line ( after the cursor )
			CaretPos = line[CaretLine -1].Text.size();
			if( line->Text.size() > 0 ){
				AppendString( CaretLine-1, CaretPos, *line );
			}
			return;
		}
	}
/*
	if( Multiline ){
		if( unicode == 13 ){ //enter
			lines.push_back( "" );

			if( BottomLine == CaretLine )
				BottomLine++;
			CaretLine++;
			CaretPos = 0;
			NumLines++;
			return;
		}
	}
*/
}

void Editbox::OnKeyRelease( int key, int mod ){
}

void Editbox::OnMouseRelease( int button ){
}
