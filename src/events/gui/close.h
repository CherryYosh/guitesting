/* 
 * File:   close.h
 * Author: brandon
 *
 * Created on May 31, 2009, 6:22 PM
 */

#ifndef _CLOSE_H
#define	_CLOSE_H

#include "guievent.h"
#include "../event.h"

class CloseEvent : public GUIEvent {
public:
	CloseEvent();
	CloseEvent(Control*);
	CloseEvent(const CloseEvent& orig);
	virtual ~CloseEvent();

	Event* clone();

	void Begin();
	void End(bool = true);
	void Step(unsigned int);

	void Linear(unsigned int);
private:
};

#endif	/* _CLOSE_H */

