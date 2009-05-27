#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>

class Event {
public:

	Event() { };

	virtual ~Event() { };

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
