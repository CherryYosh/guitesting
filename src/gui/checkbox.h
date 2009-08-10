/* 
 * File:   checkbox.h
 * Author: brandon
 *
 * Created on May 2, 2009, 5:23 PM
 */

#ifndef CHECKBOX_H
#define	CHECKBOX_H

#include "Widget.h"

class Checkbox : public Widget {
public:
	Checkbox(Window* = NULL, Widget* = NULL, LayerT = DEFAULT_LAYER, float = 0, float = 0);
	virtual ~Checkbox();
private:
};

#endif	/* _CHECKBOX_H */

