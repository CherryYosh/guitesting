/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson
 */
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

Button::Button( std::string t, Window* p, Control* c, float x, float y ) : Control(t,p,c,x,y){
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

void Button::OnMousePress( unsigned short button, int mx, int my ){
}

bool Button::OnMouseClick( unsigned short num, bool final ){
	if( num == 1 && m_Callback != NULL ){
		m_Callback();
		return false; //cant handel any more
	}

	return false; //we dont do anything
}

void Button::OnMouseEnter(){
	Root->Animate( RGBACHANNEL, nv::vec4<float>(1.0, 0.0, 0.0, 0.0), 0, 500, LINEAR, this );
}

void Button::OnMouseLeave(){
	SetColor( 0, 0, 0, 0 );
//	Root->UpdateControl( this );
	Root->RemoveAnimation( this );
}
