/* 
 * File:   guievent.h
 * Author: brandon
 *
 * Created on May 31, 2009, 4:21 PM
 */

#ifndef _GUIEVENT_H
#define	_GUIEVENT_H

#include "../../gui/control.h"
#include "../../gui/window.h"

class GUIEvent {
public:

	GUIEvent() : object(NULL) { };

	GUIEvent(Control* o) : object(o) { };

	virtual ~GUIEvent() { };

	virtual void SetObject(Control*) = 0;
protected:
	Control* object;
};

#endif	/* _GUIEVENT_H */

