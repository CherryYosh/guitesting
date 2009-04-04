#include "editbox.h"

Editbox::Editbox( std::string t, int x, int y ) : Control(t){
	this->x = x;
	this->y = y;

	text = "";
	caretPos = 0;

	modelview.set_translate( nv::vec3<float>( x, y, -1.0 ) );

	attributes |= CTRL_INPUT;
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
		printf( "%s\n", text.c_str() );
	}
}

void Editbox::onKeyRelease( int key, int mod ){
}

void Editbox::onMousePress( int button ){
}

void Editbox::onMouseRelease( int button ){
}
