/* 
 * File:   rotation.h
 * Author: brandon
 *
 * Created on May 31, 2009, 9:29 PM
 */

#ifndef _ROTATION_H
#define	_ROTATION_H

#include "guievent.h"
#include "../event.h"

class RotationEvent : public GUIEvent  {
public:
	RotationEvent();
	RotationEvent(Control*);
	RotationEvent(const RotationEvent& orig);
	virtual ~RotationEvent();

	Event* clone();
	
	void Begin();
	void End();
	void Step(unsigned int);

	void Linear(unsigned int);

	float x, y, z;
};

#endif	/* _ROTATION_H */

