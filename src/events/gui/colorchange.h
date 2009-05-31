/* 
 * File:   changecolor.h
 * Author: brandon
 *
 * Created on May 25, 2009, 9:46 PM
 */

#ifndef _CHANGECOLOR_H
#define	_CHANGECOLOR_H

#include "guievent.h"
#include "../event.h"
#include "../../utils/color.h"

class ColorChangeEvent : public Event, public GUIEvent {
public:
	ColorChangeEvent();
	ColorChangeEvent(Control*);
	ColorChangeEvent(const ColorChangeEvent& orig);
	virtual ~ColorChangeEvent();

	Event* clone();

	void SetObject(Control*);

	void Init();
	void Begin();
	void End();
	void Step(unsigned int);
	void Linear(unsigned int);

	
	void SetColor(std::string);
	util::Color GetColor();

private:
	util::Color color;
};

#endif	/* _CHANGECOLOR_H */

