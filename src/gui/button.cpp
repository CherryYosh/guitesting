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
}

void Button::OnMouseLeave(){
	SetColor( 0, 0, 0, 0 );
	Parent->UpdateControl( this );
	Parent->RemoveAnimation( this );
}
