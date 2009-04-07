#include "editbox.h"
#include "../fontmgr.h"


Editbox::Editbox( std::string t, int x, int y ) : Textbox( t, x, y ){
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
		for( unsigned short i = 0; i < size; i++ ){
			//first a check to get the line
			if( mY < y + ( height * ( TextHeight - i ) ) ){
				CaretLine = i;
				std::string line = lines[i];
				for( unsigned short j = line.length(); j >= 0; j-- ){
					line.erase( j );
					if( mX > x + ( FontMgr_GetStringWidth(font, line.c_str() ))){
						CaretPos = j;
						return true;
					}
				}
				//so if we atr here its in the line, but not the text sooo....
				CaretPos = line.length();
				return true;
			}
		}
		return true; //didnt click the text, but still clicked it..
	}
	return false;
}

void Editbox::onKeyPress( unsigned short unicode ){ 
	if( unicode > 31 && unicode < 126 && NumCharacters < MaxCharacters){
		//WARNING: Error might occure with the wunicode
		
		lines[CaretLine].insert( CaretPos++, (const char*)&unicode );
		NumCharacters++;
	
		//find a better way to do this check.. ?
		if( TextWidth < FontMgr_GetStringWidth( font, lines[CaretLine].c_str() )){
			lines[CaretLine].erase( CaretPos-1 );
			
			//dont add if we dont have the 
			//NOTE: is here rather then the if statement becouse we still need to remove the last character
			//TODO: a better way?
			if( NumLines >= MaxLines )
				return;

			//only move down if we are at the bottom
			if( BottomLine == CaretLine )
				BottomLine++;

			lines.push_back( (const char*)&unicode );
			CaretLine++;
			CaretPos = 1; //needs to be once because we added the last value..
			NumLines++;
			return;
		}
	}

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
		if( unicode == 13 /* && LineCount < MaxLineCount */ ){
			lines.push_back( "" );

			if( BottomLine == CaretLine )
				BottomLine++;
			CaretLine++;
			CaretPos = 0;
			NumLines++;
			return;
		}
	}
}

void Editbox::Render(){
}

void Editbox::onKeyRelease( int key, int mod ){
}

void Editbox::onMousePress( int button ){
}

void Editbox::onMouseRelease( int button ){
}
