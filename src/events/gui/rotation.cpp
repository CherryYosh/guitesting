/* 
 * File:   rotation.cpp
 * Author: brandon
 * 
 * Created on May 31, 2009, 9:29 PM
 */

#include "rotation.h"

RotationEvent::RotationEvent() : x(0), y(0), z(0), GUIEvent() { }

RotationEvent::RotationEvent(Control* c) : x(0), y(0), z(0), GUIEvent(c) { }

RotationEvent::RotationEvent(const RotationEvent& orig) : x(orig.x), y(orig.y), z(orig.z), GUIEvent(orig) { }

RotationEvent::~RotationEvent() { }

Event* RotationEvent::clone() {
	return new RotationEvent(*this);
}

void RotationEvent::Begin() {
	remainingTime = duration;
}

void RotationEvent::Step(unsigned int step) {
	Linear(step);
	remainingTime -= step;
	object->GetRoot()->UpdateControl(object->GetRoot());
}

void RotationEvent::End(bool update) {
	object->GetRoot()->RemoveEvent(this);
}

void RotationEvent::Linear(unsigned int step) {
	if (object == NULL)
		return;

	bool stop = false;
	//first we need to scale it to UNORM length
	if (step > remainingTime) {
		step = remainingTime;
		stop = true;
	}

	float timeDelta = float(step) / float(duration);

	if (x > 0.001) object->GetRoot()->Rotate(timeDelta * x, 1.0, 0.0, 0.0);
	if (y > 0.001) object->GetRoot()->Rotate(timeDelta * y, 0.0, 1.0, 0.0);
	if (z > 0.001) object->GetRoot()->Rotate(timeDelta * z, 0.0, 0.0, 1.0);

	if (stop)
		End();
}