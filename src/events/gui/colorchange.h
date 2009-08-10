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

class ColorChangeEvent : public GUIEvent {
public:
	ColorChangeEvent();
	ColorChangeEvent(Widget*);
	ColorChangeEvent(const ColorChangeEvent& orig);
	virtual ~ColorChangeEvent();

	Event* clone();

	void Begin();
	void End(bool = true);
	void Step(unsigned int);
	void Linear(unsigned int);

	void SetColor(util::Color);
	void SetColor(std::string);
	util::Color GetColor();

private:
	util::Color color;
};

#endif	/* _CHANGECOLOR_H */

