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

Button::Button( std::string t, int x, int y ) : Control(t){
	this->x = x;
	this->y = y;
	
	modelview.set_translate( nv::vec3<float>(x,y,-1.0) );
}

Button::~Button(){
}

bool Button::HitTest( int mouseX, int mouseY ){
	if( mouseX > x && mouseX < x + width &&
		mouseY > y && mouseY < y + height ){
		return true;
	}
	return false;
}

void Button::onMousePress( int button ){
	if( button == 1 && m_Callback ) //left mouse click, TODO: enable multi buttons
		m_Callback();
}

char Button::GetType() { return GUI_TYPE_BUTTON; }
