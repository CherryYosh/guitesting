#include "editbox.h"
#include "../fontmgr.h"


Editbox::Editbox( std::string t, int x, int y ) : Textbox( t,x,y,30, 2 ){
	flashCaret = true;
	editable = true;
	multiline = true;
}

Editbox::~Editbox(){
	//nothing right now
}

bool Editbox::HitTest( int mX, int mY ){
	if( mX > x && mX < x + ( width * scale[0] ) 
			&& mY > y && mY < y + ( height * scale[1] ) ){
		
		//tell if they clicked a line
		short size = lines.size();
		short height = FontMgr_GetLineHeight( font );
		for( short i = 0; i < size; i++ ){
			//first a check to get the line
			if( mY < y + ( height * ( theight - i ) ) ){
				caretLine = i;
				std::string line = lines[i];
				for( short j = line.length(); j >= 0; j-- ){
					line.erase( j );
					if( mX > x + ( FontMgr_GetStringWidth(font, line.c_str() ))){
						caretPos = j;
						return true;
					}
				}
				//so if we atr here its in the line, but not the text sooo....
				caretPos = line.length();
				return true;
			}
		}
		return true; //didnt click the text, but still clicked it..
	}
	return false;
}

void Editbox::onKeyPress( unsigned short unicode ){ 
	if( unicode > 31 && unicode < 126 ){
		//WARNING: Error might occure with the wunicode
		
		lines[caretLine].insert( caretPos++, (const char*)&unicode );
	
		//find a better way to do this check.. ?
		if( twidth < FontMgr_GetStringWidth( font, lines[caretLine].c_str() )){
			lines[caretLine].erase( caretPos-1 );
			
			//only move down if we are at the bottom
			if( bottomLine == caretLine )
				bottomLine++;

			lines.push_back( (const char*)&unicode );
			caretLine++;
			caretPos = 1; //needs to be once because we added the last value..
			return;
		}
	}

	if( unicode == 8 && caretPos > 0){ //backspace
		lines[caretLine].erase( --caretPos );
		return;
	}

	if( unicode == 13 /* && LineCount < MaxLineCount */ ){
		lines.push_back( "" );

		if( bottomLine == caretLine )
			bottomLine++;
		caretLine++;
		caretPos = 0;
		return;
	}
}

void Editbox::Render(){
	_Shader->SetModelview( modelview._array );
	glVertexAttribPointer(_Shader->attribute[0], 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(NULL + vertexOffset));
	glVertexAttribPointer(_Shader->attribute[1], 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(NULL + textureOffset));
	glDrawElements( GL_QUADS, 4, GL_UNSIGNED_SHORT, 0 );
}

void Editbox::onKeyRelease( int key, int mod ){
}

void Editbox::onMousePress( int button ){
}

void Editbox::onMouseRelease( int button ){
}
