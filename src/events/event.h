#ifndef EVENT_H
#define EVENT_H

class Control;

#include <stdio.h>

class Event {
public:

	Event() { };
	Event(const Event& e) : start(e.start), duration(e.duration), delay(e.delay) { };

	virtual ~Event() { };

	/**
	 * Clones the event, returning a pointer to a new unique event
	 */
	virtual Event* clone() { return new Event(*this); };

	virtual void SetObject(Control*){ };

	/**
	 * Initlizes all values
	 */
	virtual void Init() { };

	/**
	 * Called when ever we start the event 
	 */
	virtual void Begin() { };

	/**
	 * Called after the event is finsihed
	 */
	virtual void End() { };

	virtual void Step(unsigned int) { };
protected:
	unsigned int start;
	unsigned int duration;
	unsigned int delay;
private:
};

#endif
