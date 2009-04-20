/**********************
 * a simple square button
 * executes a function when clicked
 *
 * TODO:
 * 	double click
 * 	text
 * 	fit to the size of the screen
 *
 * Known bugs:
 * 	none :P
 */

#include "button.h"
#include "window.h"

Button::Button( Window* p, std::string t, int x, int y ) : Control(p,t,x,y){
}

Button::~Button(){
}

bool Button::HitTest( int mouseX, int mouseY ){
	if( mouseX > x && mouseX < x + Width &&
		mouseY > y && mouseY < y + Height ){
		return true;
	}
	return false;
}

void Button::OnMousePress( int button ){
	if( button == 0 && m_Callback )
		m_Callback();
}

void Button::OnMouseEnter(){
	Parent->Animate( RGBACHANNEL, nv::vec4<float>(1.0, 1.0, 0.0, 0.0), 0, 500, LINEAR, this );
	isAnimated = true;
}

void Button::OnMouseLeave(){
	SetColor( 0, 0, 0, 0 );
	isAnimated = false;
}
