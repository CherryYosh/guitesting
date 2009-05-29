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
		endColor = util::Color(1.0,0.0,0.0,0.0);
		duration = 50;
		remainingTime = duration;
		delay = 0;
	}
}

void changecolor::End(){
}

void changecolor::Step(unsigned int step){
	Linear(step);
	remainingTime -= step;
}

void changecolor::Linear(unsigned int step){
	if(object == NULL)
		return;

	//first we need to scale it to UNORM length
	if(step > remainingTime)
		step = remainingTime;

	float timeDelta = step / duration;

	util::Color oc = object->GetColor();

	object->SetColor(util::Color(
		oc.r + (step * endColor.r),
		oc.g + (step * endColor.g),
		oc.b + (step * endColor.b),
		oc.a + (step * endColor.a)));
}
