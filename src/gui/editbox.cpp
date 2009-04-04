#include "editbox.h"

Editbox::Editbox( std::string t, int x, int y ) : Textbox( t,x,y,width,height){
}

Editbox::~Editbox(){
	//nothing right now
}

bool Editbox::HitTest( int mX, int mY ){
	if( mX > x && mY < x + ( width * scale[0] ) 
			&& mY > y && mY < y + ( height * scale[1] ) ){
		return true;
	}
	return false;
}

void Editbox::onKeyPress( int key, int mod ){
	if( key  > 31 && key < 126 ){
		text.insert( caretPos++, (const char*)&key );
	}

	if( key == 8 ){ //backspace
		text.erase( caretPos-- );
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
