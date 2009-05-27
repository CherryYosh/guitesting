/* 
 * File:   changecolor.cpp
 * Author: brandon
 * 
 * Created on May 25, 2009, 9:46 PM
 */

#include "changecolor.h"

changecolor::changecolor() { }

changecolor::changecolor(Control* o){
	object = o;
}

changecolor::changecolor(const changecolor& orig) { }

changecolor::~changecolor() { }

void changecolor::Init(){}

void changecolor::Begin(){
	startColor = object->GetColor();
	endColor = nv::vec4<float>(1.0,0.0,0.0,0.0);
	duration = 50;
	delay = 0;
}

void changecolor::End(){
}

void changecolor::Step(unsigned int step){
	object->SetColor(1.0,0.0,0.0,0.0);
}

