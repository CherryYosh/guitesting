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
#include "../../nvVector.h"

class changecolor : public Event {
public:
	changecolor();
	changecolor(Control*);
	changecolor(const changecolor& orig);
	virtual ~changecolor();

	void Init();
	void Begin();
	void End();
	void Step(unsigned int);
private:
	nv::vec4<float> startColor;
	nv::vec4<float> endColor;
	Control* object;

};

#endif	/* _CHANGECOLOR_H */

