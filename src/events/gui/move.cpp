/* 
 * File:   move.cpp
 * Author: brandon
 * 
 * Created on May 26, 2009, 2:41 PM
 */

#include "move.h"

MoveEvent::MoveEvent() : delta(0, 0), GUIEvent() { }

MoveEvent::MoveEvent(Control* c) : delta(0, 0), GUIEvent(c) { }

MoveEvent::MoveEvent(const MoveEvent& orig) : delta(orig.delta), GUIEvent(orig) { }

MoveEvent::~MoveEvent() { }

Event* MoveEvent::clone() {
	return new MoveEvent(*this);
}

void MoveEvent::Begin() {
	if (object != NULL) {
		remainingTime = duration;
	}
};

void MoveEvent::End() {
	object->GetRoot()->RemoveEvent(this);
};

void MoveEvent::Step(unsigned int step) {
	Linear(step);
	remainingTime -= step;
	object->GetRoot()->UpdateControl(object);
}

void MoveEvent::Linear(unsigned int step) {
	if (object == NULL)
		return;

	bool stop = false;
	//first we need to scale it to UNORM length
	if (step > remainingTime) {
		step = remainingTime;
		stop = true;
	}

	float timeDelta = float(step) / float(duration);

	float x = object->GetX();
	float y = object->GetY();

	printf("moving from %f %f\n", x, y);

	object->Move(
		x + (timeDelta * delta.x),
		y + (timeDelta * delta.y)
		);

	if (stop)
		object->GetRoot()->RemoveEvent(this);
}

void MoveEvent::SetObject(Control* c) {
	object = c;
}

void MoveEvent::Translation(int x, int y) {
	delta = util::vec2<float>(x, y);
}