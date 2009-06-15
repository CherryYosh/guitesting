/* 
 * File:   close.cpp
 * Author: brandon
 * 
 * Created on May 31, 2009, 6:22 PM
 */

#include "close.h"

CloseEvent::CloseEvent() : GUIEvent() { };

CloseEvent::CloseEvent(Control* c) : GUIEvent(c) { }

CloseEvent::CloseEvent(const CloseEvent& orig)  : GUIEvent(orig) { };

CloseEvent::~CloseEvent() { };

Event* CloseEvent::clone(){
	return new CloseEvent(*this);
}

void CloseEvent::Begin(){
	remainingTime = duration;
}

void CloseEvent::Step(unsigned int step){
	if(object != NULL){
		object->GetRoot()->Close();
		End();
	} else
		printf("FAIL!!!\n");
}

void CloseEvent::End(){
	
}

void CloseEvent::Linear(unsigned int){}

