#include "editbox.h"
#include "../fontmgr.h"


Editbox::Editbox( std::string t, int x, int y ) : Label( t, x, y ){
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
				unsigned int j = line.Size;
				for( it=line.Text.end(); it != line.Text.begin(); it--){
					if( mX > x + ((FontChar)*it).x ){
						CaretPos = j;
						return true;
					}
					
					j--;
				}
				
				//so if we are here its in the line, but not the text sooo....
				CaretPos = line.Size;
				return true;
			}
		}
		//So that bastard doesnt want text!!!
		return true;
	}
	return false;
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
		char p = ( CaretPos > 0 ) ? (*--line->Text.end()).c : 0;
		float h;
		FontMgr_GetCharData( line->font, p, h, c );


		//check if the new chara needs to go on a new line
		if( TextWidth < line->Width + c.width ){
			//dont add if we dont have the space 
			if( NumLines >= MaxLines )
				return;

			//only move down if we are at the bottom
			if( BottomLine == CaretLine )
				BottomLine++;
/*
			FontString s;
			s.font = 0;
			s.Start = line->Start + line->Length;
			s.Length = 0;
			s.Size = 1;
			s.y = line->y + line->Height;
			s.Width = c.width;
			s.Height = FontMgr_GetLineHeight( s.font );
			s.Text.push_back( c );
*/
//			line->.push_back( s );
			CaretLine++;
			CaretPos = 1; //needs to be once because we added the last value..
			NumLines++;
			NumCharacters++;

			//RebuildVBO();
			return;
		}
		
		//otherwise we can add it to the end of the line
		ReplaceCharVBO( c );
		line->Width += /*c.width +*/ h;
		std::list<FontChar>::iterator it = line->Text.begin();
		std::advance( it, CaretPos++ );
		line->Text.insert(it, c);
		NumCharacters++;

		//RebuildVBO();
		return;
	}
/*
	if( unicode == 8 && CaretPos > 0){ //backspace
		lines[CaretLine].erase( --CaretPos );
		NumCharacters--;
		return;
	} else if( Multiline && CaretLine > 0 ){
		//backspaced pressed at the end of the line..
		//all this does and delet the current line, moving its 
		//content to the new line ( after the cursor )
		CaretPos = lines[CaretLine -1].length();
		if( lines[CaretLine].size() > 0 ){
			lines[CaretLine -1].append( lines[CaretLine] );
		}

		if( BottomLine == CaretLine )
			BottomLine--;

		std::vector<std::string>::iterator iter = lines.begin();
		iter += CaretLine--;
		lines.erase( iter );
		NumLines--;
		return;
	}

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

/*
void Editbox::Render(){
}
*/
void Editbox::OnKeyRelease( int key, int mod ){
}

void Editbox::OnMousePress( int button ){
}

void Editbox::OnMouseRelease( int button ){
}
