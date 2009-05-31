/* 
 * File:   changecolor.cpp
 * Author: brandon
 * 
 * Created on May 25, 2009, 9:46 PM
 */

#include "colorchange.h"

ColorChangeEvent::ColorChangeEvent() : color("#000000FF"), GUIEvent(), Event() {}

ColorChangeEvent::ColorChangeEvent(Control* o) : color("#000000"), GUIEvent(o), Event() {}

ColorChangeEvent::ColorChangeEvent(const ColorChangeEvent& c) : color(c.color), GUIEvent(c.object), Event(c) { }

ColorChangeEvent::~ColorChangeEvent() { }

Event* ColorChangeEvent::clone() {
	return new ColorChangeEvent(*this);
}

void ColorChangeEvent::SetObject(Control* o) {
	object = o;
}

void ColorChangeEvent::Init() { }

void ColorChangeEvent::Begin() {
	if (object != NULL) {
		remainingTime = duration;
	}
}

void ColorChangeEvent::End() {
	object->SetColor(util::Color(0.0, 0.0, 0.0, 0.0));
	object->GetRoot()->RemoveEvent(this);
}

void ColorChangeEvent::Step(unsigned int step) {
	Linear(step);
	remainingTime -= step;
	object->GetRoot()->UpdateControl(object);
}

void ColorChangeEvent::Linear(unsigned int step) {
	if (object == NULL)
		return;

	bool stop = false;
	//first we need to scale it to UNORM length
	if (step > remainingTime) {
		step = remainingTime;
		stop = true;
	}

	float timeDelta = float(step) / float(duration);

	util::Color oc = object->GetColor();

	object->SetColor(util::Color(
		oc.r + (timeDelta * color.r),
		oc.g + (timeDelta * color.g),
		oc.b + (timeDelta * color.b),
		oc.a + (timeDelta * color.a)
		));

	if (stop)
		object->GetRoot()->RemoveEvent(this);
}

void ColorChangeEvent::SetColor(std::string s) {
	color = util::Color(s);
}

util::Color ColorChangeEvent::GetColor() {
	return color;
}
