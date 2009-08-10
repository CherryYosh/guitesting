/* 
 * File:   guievent.h
 * Author: brandon
 *
 * Created on May 31, 2009, 4:21 PM
 */

#ifndef _GUIEVENT_H
#define	_GUIEVENT_H

#include "../event.h"
#include "../../gui/widget.h"
#include "../../gui/window.h"

class GUIEvent : public Event {
public:

	GUIEvent() : object(NULL), Event() { };

	GUIEvent(Widget* o) : object(o), Event() { };

	GUIEvent(const GUIEvent& orig) : object(orig.object), Event(orig) { };

	virtual ~GUIEvent() { };

	virtual void SetObject(Widget* c) {
		object = c;
	};

	Widget* GetObject(){
		return object;
	}
protected:
	Widget* object;
};

#endif	/* _GUIEVENT_H */

