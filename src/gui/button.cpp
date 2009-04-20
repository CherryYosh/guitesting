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
	if( button == 1 && m_Callback ) //left mouse click, TODO: enable multi buttons
		m_Callback();
}

void Button::OnMouseOver(){
	Parent->Animate( RGBACHANNEL, nv::vec4<float>(1.0, 1.0, 0.0, 0.0), 0, 500, LINEAR, this );
	isAnimated = true;
}
