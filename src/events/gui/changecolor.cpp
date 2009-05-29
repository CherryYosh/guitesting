/* 
 * File:   changecolor.cpp
 * Author: brandon
 * 
 * Created on May 25, 2009, 9:46 PM
 */

#include "changecolor.h"

changecolor::changecolor() : object(NULL) { }

changecolor::changecolor(Control* o){
	object = o;
}

changecolor::changecolor(const changecolor& c) : startColor(c.startColor),
	endColor(c.endColor), Event(c) { }

changecolor::~changecolor() { }

Event* changecolor::clone() { return new changecolor(*this); }

void changecolor::SetObject(Control* o){
	object = o;
}

void changecolor::Init(){}

void changecolor::Begin(){
	if(object != NULL){
		startColor = object->GetColor();
		endColor = nv::vec4<float>(1.0,0.0,0.0,0.0);
		duration = 50;
		delay = 0;
	}
}

void changecolor::End(){
}

void changecolor::Step(unsigned int step){
	if(object != NULL){
		object->SetColor(1.0,0.0,0.0,0.0);
	}
}

