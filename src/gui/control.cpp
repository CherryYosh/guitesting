#include "control.h"

Control::Control( std::string t ){
	isEnabled = true;
	hasFocus = false;
	type = t;
	modelview.make_identity();
	
	scale[0] = 1.0;
	scale[1] = 1.0;

	attributes = 0;
}

Control::~Control(){
	//is there anything to do??
}

void Control::Activate(){
	//nothing needs to be done
}

void Control::Render(){
	_Shader->SetModelview( modelview._array );
	glVertexAttribPointer(_Shader->attribute[0], 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(NULL + vertexOffset));
	glVertexAttribPointer(_Shader->attribute[1], 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(NULL + textureOffset));
	glDrawElements( GL_QUADS, 4, GL_UNSIGNED_SHORT, 0 );
}

bool Control::HitTest( int mouseX, int mouseY ){
	return false;
}

void Control::onMousePress( int button ){
	//called by input
}

void Control::onMouseRelease( int button ){
	//called by input
}

void Control::onKeyPress( int key, int mod ){
	//called by input
}

void Control::onKeyRelease( int key, int mod ){
	//called by input
}

void Control::Move( int cx, int cy ){
	x += cx;
	y += cy;
	modelview.set_translate( nv::vec3<float>(x, y, -1.0) );	
}

void Control::SetEnabled( bool value ){
	isEnabled = value;
}

void Control::SetFocuse( bool value ){
	hasFocus = value;
}

void Control::SetCallback( boost::function<int()> callback ){
	m_Callback = callback;
}

void Control::SetWidth( unsigned int w ){
	width = w;
}

void Control::SetHeight( unsigned int h ){
	height = h;
}

bool Control::HasAttrib( unsigned short a ){
	//will only return true if all the attributes are there
	return a == ( attributes & a );
}
