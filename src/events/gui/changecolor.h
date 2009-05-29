/* 
 * File:   changecolor.h
 * Author: brandon
 *
 * Created on May 25, 2009, 9:46 PM
 */

#ifndef _CHANGECOLOR_H
#define	_CHANGECOLOR_H

#include "../event.h"
#include "../../gui/control.h"
#include "../../utils/color.h"

class changecolor : public Event {
public:
	changecolor();
	changecolor(Control*);
	changecolor(const changecolor& orig);
	virtual ~changecolor();

	Event* clone();

	void SetObject(Control*);

	void Init();
	void Begin();
	void End();
	void Step(unsigned int);
	void Linear(unsigned int);
private:
	util::Color startColor;
	util::Color endColor;
	Control* object;
};

#endif	/* _CHANGECOLOR_H */

