/* 
 * File:   move.h
 * Author: brandon
 *
 * Created on May 26, 2009, 2:41 PM
 */

#ifndef _MOVE_H
#define	_MOVE_H

#include "guievent.h"
#include "../event.h"

class MoveEvent : public Event, public GUIEvent{
public:
	MoveEvent();
	MoveEvent(Control*);
	MoveEvent(const MoveEvent& orig);
	virtual ~MoveEvent();

	Event* clone();

	void SetObject(Control*);

	void Init();
	void Begin();
	void End();
	void Step(unsigned int);

	void Linear(unsigned int);
	void Translation(int, int);
private:
	util::vec2<float> delta;
};

#endif	/* _MOVE_H */

