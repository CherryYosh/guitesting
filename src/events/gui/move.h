/* 
 * File:   move.h
 * Author: brandon
 *
 * Created on May 26, 2009, 2:41 PM
 */

#ifndef _MOVE_H
#define	_MOVE_H

#include "../event.h"
#include "../../gui/control.h"

class Move : public Event{
public:
	Move();
	Move(const Move& orig);
	virtual ~Move();

	void Init();
	void Begin();
	void End();
	void Step(unsigned int);
private:
	nv::vec3<float> startPoint;
	nv::vec3<float> endPoint;
};

#endif	/* _MOVE_H */

