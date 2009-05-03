/* 
 * File:   checkbox.cpp
 * Author: brandon
 * 
 * Created on May 2, 2009, 5:23 PM
 */

#include "checkbox.h"

Checkbox::Checkbox(std::string t, Window* p, Control* c, float x, float y ) : Control(t,p,c,x,y){
	Control* c2 = new Control( "checkbox-check", p, this );
	c2->SetColor(0.0, 0.0, 0.0, -1.0); //make it invis, need a better way
	Children.push_back( c2 );

	isChecked = false;
}

Checkbox::~Checkbox() { }

bool Checkbox::OnMouseClick(unsigned short, bool){
	isChecked = !isChecked;
	Children[0]->SetColor( 0.0, 0.0, 0.0, -1.0 + isChecked );

	if( m_Callback != NULL ){
	    m_Callback();
	}

	return true;
}
