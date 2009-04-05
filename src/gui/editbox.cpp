#include "editbox.h"
#include "../fontmgr.h"


Editbox::Editbox( std::string t, int x, int y ) : Textbox( t,x,y,width,height){
	flashCaret = true;
	editable = true;
}

Editbox::~Editbox(){
	//nothing right now
}

bool Editbox::HitTest( int mX, int mY ){
	if( mX > x && mX < x + ( width * scale[0] ) 
			&& mY > y && mY < y + ( height * scale[1] ) ){
		
		//now to handel
		if( mY < y + ( FontMgr_GetLineHeight( font ) * theight ) &&
				mX < x + ( FontMgr_GetStringWidth(font, text.c_str() ))) { //note mY > y has already been checked
			
			std::string temp = text;
			for( int i = text.length(); i >= 0; i-- ){
				temp.erase( i ); 
				if( mX > x + FontMgr_GetStringWidth(font, temp.c_str() )){
					caretPos = i;
					printf( "caret found at %i\n", caretPos );
					break;
				}
			}
		}
		
		return true;
	}
	return false;
}

void Editbox::onKeyPress( unsigned short unicode ){ 
	if( unicode > 31 && unicode < 126 ){
		//WARNING: Error might occure with the wunicode
		text.insert( caretPos++, (const char*)&unicode );
	}

	if( unicode == 8 && caretPos > 0){ //backspace
		text.erase( --caretPos );
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
