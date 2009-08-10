#ifndef EVENT_H
#define EVENT_H

class Widget;

#include <stdio.h>

class Event {
public:

	Event() : start(0), duration(1000), delay(0), remainingTime(0) { };

	Event(const Event& e) : start(e.start), duration(e.duration), delay(e.delay),
		remainingTime(e.remainingTime){ };

	virtual ~Event() { };

	/**
	 * Clones the event, returning a pointer to a new unique event
	 */
	virtual Event* clone() = 0;

	/**
	 * Called when ever we start the event 
	 */
	virtual void Begin() = 0;

	/**
	 * Called after the event is finsihed
	 */
	virtual void End(bool = true) = 0;

	virtual void Step(unsigned int) = 0;
protected:
	unsigned int start;
	unsigned int duration;
	unsigned int delay;
	unsigned int remainingTime;
private:
};

#endif
